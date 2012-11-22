/*
 * Copyright (c) 2012 - Guilhem Bonnefille <guilhem/bonnefille@gmail.com>
 *
 * This file is part of the ref:lect project. ref:lect is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * ref:lect is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "mirror.h"

#include <security/pam_modules.h>
#include <security/_pam_macros.h>
#include <security/pam_ext.h>

#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>
#include <dbus/dbus-glib-lowlevel.h>

#include "pam_utils.h"

static gboolean tag_match;
static gboolean timeout_occured;
static char *waited_tag;

static void
create_framework (pam_handle_t *pamh, DBusGConnection **ret_conn, GMainLoop **ret_loop)
{
	DBusGConnection *connection;
	DBusConnection *conn;
	DBusError error;
	GMainLoop *loop;
	GMainContext *ctx;

	/* Otherwise dbus-glib doesn't setup it value types */
	connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, NULL);

	if (connection != NULL)
		dbus_g_connection_unref (connection);

	/* And set us up a private D-Bus connection */
	dbus_error_init (&error);
	conn = dbus_bus_get_private (DBUS_BUS_SYSTEM, &error);
	if (conn == NULL) {
		ERR (pamh, "Error with getting the bus: %s", error.message);
		dbus_error_free (&error);
		return;
	}

	/* Set up our own main loop context */
	ctx = g_main_context_new ();
	loop = g_main_loop_new (ctx, FALSE);
	dbus_connection_setup_with_g_main (conn, ctx);

	connection = dbus_connection_get_g_connection (conn);

	*ret_conn = connection;
	*ret_loop = loop;
}

static void
destroy_framework (DBusGConnection *gconn, GMainLoop *gloop)
{
	DBusConnection *conn;

	conn = dbus_g_connection_get_connection (gconn);
	dbus_connection_close (conn);
	dbus_g_connection_unref (gconn);
	
	
	g_main_loop_unref (gloop);

}

static DBusGProxy *
create_mirror (pam_handle_t *pamh, DBusGConnection *gconn)
{
	DBusGProxy *proxy;

	/* Create a proxy object for the "bus driver" (name "org.freedesktop.DBus") */
	proxy = dbus_g_proxy_new_for_name (gconn,
	                                   "org.rfid.Mirror",
	                                   "/org/rfid/mirror",
	                                   "org.rfid.Mirror"); 
	return proxy;
}

static void
tag_enter_cb (DBusGProxy *proxy,
		 char *tag,
		 gpointer user_data)
{
	GError *error = NULL;
	GMainLoop *mainloop = (GMainLoop *)user_data;

	// 
	if (tag != NULL && waited_tag != NULL &&
		strcmp (tag, waited_tag) == 0) {
		tag_match = TRUE;
	}
	
	// stop mainloop
	g_main_loop_quit (mainloop);

}

static gboolean
timeout_cb (gpointer user_data)
{
	GMainLoop *mainloop = (GMainLoop *)user_data;
	
	// stop mainloop
	if (g_main_loop_is_running (mainloop))
		g_main_loop_quit (mainloop);
	
	timeout_occured = TRUE;
	
	return FALSE;
}

int wait_token(pam_handle_t *pamh, char *tag, int delay) {
	DBusGConnection *connection = NULL;
	GError *error = NULL;
	DBusGProxy *proxy = NULL;
	GMainLoop *mainloop = NULL;
	GSource *source = NULL;
	
	g_type_init ();
	
	tag_match = FALSE;
	timeout_occured = FALSE;
	waited_tag = tag;
	
	error = NULL;

	create_framework (pamh, &connection, &mainloop);
	if (connection == NULL) {
		g_error_free (error);
		return PAM_AUTHINFO_UNAVAIL;
	}
	
	/* Create a proxy object for the "bus driver" (name "org.freedesktop.DBus") */
	proxy = create_mirror (pamh, connection); 
	                                   
	/* Expect signal about tag entering */
	dbus_g_proxy_add_signal (proxy, "TagEnter",
				 G_TYPE_STRING,
				 G_TYPE_INVALID);
	dbus_g_proxy_connect_signal (proxy, "TagEnter",
				     G_CALLBACK (tag_enter_cb),
				     (gpointer)mainloop, NULL);

	/* Set up the timeout on our non-default context */
	source = g_timeout_source_new_seconds (delay);
	g_source_attach (source, g_main_loop_get_context (mainloop));
	g_source_set_callback (source, timeout_cb, mainloop, NULL);
				     
	g_main_loop_run (mainloop);

	g_source_destroy (source);
	g_source_unref (source);
	
	dbus_g_proxy_disconnect_signal (proxy, "TagEnter",
			G_CALLBACK (tag_enter_cb),
			(gpointer)mainloop);

	g_object_unref (proxy);

	destroy_framework (connection, mainloop);


	if (tag_match)
		return PAM_SUCCESS;
	else if (timeout_occured)
		return PAM_AUTHINFO_UNAVAIL;
	else
		/* Token does not match */
		return PAM_AUTH_ERR;
}

int check_token(pam_handle_t *pamh, char *stored_tag) {
	DBusGConnection *connection;
	GError *error;
	DBusGProxy *proxy;
	gchar **tags = NULL;
	gchar **tag = NULL;
	gboolean found = FALSE;

	g_type_init ();

	error = NULL;
	connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);
	if (connection == NULL) {
		g_error_free (error);
		return PAM_AUTHINFO_UNAVAIL;
	}

	/* Create a proxy object for the "bus driver" (name "org.freedesktop.DBus") */
	proxy = create_mirror (pamh, connection); 

	/* Call getTags method, wait for reply */
	error = NULL;
	if (!dbus_g_proxy_call_with_timeout (proxy, "GetTags", 1*60*1000, &error,
	                                     G_TYPE_INVALID,
	                                     G_TYPE_STRV, &tags,
	                                     G_TYPE_INVALID))
	{
		/* Just do demonstrate remote exceptions versus regular GError */
		g_error_free (error);
		g_object_unref (proxy);
		return PAM_AUTHINFO_UNAVAIL;
	}

	/* Check the results */
	tag = tags;
	while (!found && tag != NULL && *tag != NULL && **tag != '\0') {
		if (strcmp (*tag, stored_tag) == 0)
			found = TRUE;
		tag++;
	}

	g_object_unref (proxy);

	dbus_g_connection_unref (connection);


	if (found)
		return PAM_SUCCESS;
	else if (tags == NULL || *tags == NULL)
		/* No tag available */
		return PAM_AUTHINFO_UNAVAIL;
	else
		return PAM_AUTH_ERR;
}

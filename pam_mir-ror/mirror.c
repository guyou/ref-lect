#include "mirror.h"

#include <security/pam_modules.h>
#include <security/_pam_macros.h>
#include <security/pam_ext.h>

#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>


int check_token(char *stored_tag) {
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
	proxy = dbus_g_proxy_new_for_name (connection,
	                                   "org.rfid.Mirror",
	                                   "/org/rfid/mirror",
	                                   "org.rfid.Mirror"); 

	/* Call getTags method, wait for reply */
	error = NULL;
	if (!dbus_g_proxy_call_with_timeout (proxy, "getTags", 1*60*1000, &error,
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
	return found ? PAM_SUCCESS : PAM_AUTH_ERR;
}

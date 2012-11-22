/*
 * pam_fprint: PAM module for fingerprint authentication through fprintd
 * Copyright (C) 2007 Daniel Drake <dsd@gentoo.org>
 * Copyright (C) 2008 Bastien Nocera <hadess@hadess.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <glib.h>
#include <security/pam_modules.h>

gboolean send_info_msg(pam_handle_t *pamh, const char *msg);
gboolean send_err_msg(pam_handle_t *pamh, const char *msg);
void send_debug_msg(pam_handle_t *pamh, const char *msg);

#define INFO(pamh, ...) {                                       \
        char *s;                                \
        s = g_strdup_printf (__VA_ARGS__);      \
        send_info_msg (pamh, s);                \
        g_free (s);                             \
}

#define ERR(pamh, ...) {                                        \
        char *s;                                \
        s = g_strdup_printf (__VA_ARGS__);      \
        send_err_msg (pamh, s);         \
        g_free (s);                             \
}

#define DEBUG(pamh, ...) {					\
	if (debug) {					\
		char *s;				\
		s = g_strdup_printf (__VA_ARGS__);	\
		send_debug_msg (pamh, s);		\
		g_free (s);				\
	}						\
}


extern gboolean debug;


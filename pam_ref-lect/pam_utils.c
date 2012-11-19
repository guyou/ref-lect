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

#include "pam_utils.h"

#include <glib.h>
#include <syslog.h>

#include <security/pam_modules.h>

gboolean debug = FALSE;

gboolean send_info_msg(pam_handle_t *pamh, const char *msg)
{
        const struct pam_message mymsg = {
                .msg_style = PAM_TEXT_INFO,
                .msg = msg,
        };
        const struct pam_message *msgp = &mymsg;
        const struct pam_conv *pc;
        struct pam_response *resp;
        int r;

        r = pam_get_item(pamh, PAM_CONV, (const void **) &pc);
        if (r != PAM_SUCCESS)
                return FALSE;

        if (!pc || !pc->conv)
                return FALSE;

        return (pc->conv(1, &msgp, &resp, pc->appdata_ptr) == PAM_SUCCESS);
}

gboolean send_err_msg(pam_handle_t *pamh, const char *msg)
{
        const struct pam_message mymsg = {
                .msg_style = PAM_ERROR_MSG,
                .msg = msg,
        };
        const struct pam_message *msgp = &mymsg;
        const struct pam_conv *pc;
        struct pam_response *resp;
        int r;

        r = pam_get_item(pamh, PAM_CONV, (const void **) &pc);
        if (r != PAM_SUCCESS)
                return FALSE;

        if (!pc || !pc->conv)
                return FALSE;

        return (pc->conv(1, &msgp, &resp, pc->appdata_ptr) == PAM_SUCCESS);
}

void send_debug_msg(pam_handle_t *pamh, const char *msg)
{
        gconstpointer item;
        const char *service;

        if (pam_get_item(pamh, PAM_SERVICE, &item) != PAM_SUCCESS || !item)
                service = "<unknown>";
        else
                service = item;

        openlog (service, LOG_CONS | LOG_PID, LOG_AUTHPRIV);

        syslog (LOG_AUTHPRIV|LOG_WARNING, "%s(%s): %s", "pam_reflect", service, msg);

        closelog ();

}


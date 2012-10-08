/*
 * Copyright (c) 2009 - Joël PASTRE 'Jopa'  <joel@jopa.fr>
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


#include <security/pam_modules.h>
#include <security/_pam_macros.h>
#include <security/pam_ext.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>
#include <asm/types.h>

#include <termios.h>

#include <pwd.h>

#include "mirror.h"

#define PAM_SM_AUTH

#define TAGFILE	".authtag"

#define WAIT_KEY "wait"
#define NOWAIT_KEY "nowait"

//-----------------------------------------------------------------------------
// get_user_tagfile : Build ~/.auth path using user name and passwd file.
// 		      return filename with complete path.
static
void get_user_tagfile(char *user, char *tagfile) {
	struct passwd *pw;
	pw = getpwnam (user);
	sprintf(tagfile,"%s/%s",pw->pw_dir,TAGFILE);
}

//
static int _pam_parse(int argc, const char **argv)
{
     int wait_delay=-1;

     /* step through arguments */
     for (; argc-- > 0; ++argv) {

          /* generic options */

          if (!strcmp(*argv,NOWAIT_KEY))
               wait_delay = -1;
          else if (!strncmp(*argv,WAIT_KEY"=",sizeof(WAIT_KEY))) {
               int glen = strlen(*argv);
               wait_delay = atoi(*argv+sizeof(WAIT_KEY));
          } else {
               syslog(LOG_ERR,"pam_parse: unknown option; %s",*argv);
          }
     }

     return wait_delay;
}

//----------------------------------------------------------------------------
// pam_sm_authenticate : pam function - user authentification
PAM_EXTERN
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	int retval;
	int wait_delay;
	const char *service;
	const char *user;
	const char *tty;

	char device[50];

	char stored_tag[25];
	char *ptr;

	char tag_file_path[256];
	FILE *tagfile;

	int term_isatty;
	struct termios term_attr;

	pthread_t prompt;

	openlog ("[pam_reflect]", LOG_PID, LOG_AUTH);

	wait_delay = _pam_parse(argc, argv);

	term_isatty = isatty(STDIN_FILENO);

	//Save term Attributes
	if (term_isatty==1) {
		tcgetattr(STDIN_FILENO,&term_attr);
	}

	//Get service
	retval = pam_get_item(pamh, PAM_SERVICE, (const void **)(const void *)&service);

	if (retval != PAM_SUCCESS) {
		syslog(LOG_WARNING, "Unable to retrieve the PAM service name.");
		return PAM_AUTH_ERR;
		closelog();
	}

	//Get user
	if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS || !user || !*user) {
		syslog(LOG_WARNING, "Unable to retrieve the PAM user name.");
		closelog();
		return PAM_AUTH_ERR;
	}

	syslog(LOG_WARNING,"Authentification request for user '%s' (%s)\n",user,service);

	//No rfid use for ssh tty
	if (pam_get_item(pamh, PAM_TTY, (const void **)(const void *)&tty) == PAM_SUCCESS) {
		if (tty && !strcmp(tty,"ssh")) {
			syslog(LOG_WARNING,"Not using RFID for SSH Authentification.");
			closelog();
			return PAM_AUTH_ERR;
		}
	}

	//User .authtag file
	get_user_tagfile((char *)user,tag_file_path);
	tagfile = fopen (tag_file_path,"r");

	if (tagfile==NULL) {
		syslog(LOG_WARNING,"Unable to open rfid tag file : %s, for user : %s",tag_file_path,user);
		closelog();
		return PAM_SERVICE_ERR;
	}	

	if (fgets(stored_tag,25,tagfile) == NULL) {
		syslog(LOG_WARNING,"Unable to read rfid tag file : %s, for user : %s",tag_file_path,user);
		closelog();
		return PAM_SERVICE_ERR;
	}
	// Clean read: remove not allowed characters
	ptr = stored_tag;
	while (*ptr != '\0') {
		if (strchr("ABCDEF0123456789", *ptr) == NULL)
			*ptr = '\0';
		ptr++;
	}

	// Compare stored tag with ztamp:s tag
	if (check_token (stored_tag)==0) {
		syslog(LOG_WARNING,"Authentification granted for user '%s' (%s)",user,service);
		closelog();
		return PAM_SUCCESS;

	} else if (wait_delay > 0 && wait_token (stored_tag, wait_delay)==0) {
		syslog(LOG_WARNING,"Authentification granted for user '%s' (%s)",user,service);
		closelog();
		return PAM_SUCCESS;

	} else {
		syslog(LOG_WARNING,"Authentification failure for user '%s' (%s)",user,service);
		closelog();

		return PAM_AUTH_ERR;
	}

}//pam_sm_authenticate

PAM_EXTERN
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	return PAM_SUCCESS;
}

PAM_EXTERN
int pam_sm_chauthok(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	return PAM_SUCCESS;
}

#ifdef PAM_STATIC
struct pam_module _pam_reflect_modstruct = {
	"pam_reflect",
	pam_sm_authenticate,
	pam_sm_setcred,
	NULL,
	NULL,
	NULL,
	pam_sm_chauthtok
};

#endif

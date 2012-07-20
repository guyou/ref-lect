// pam_mir-ror.c
/*
 * Copyright (c) 2009 - Joël PASTRE 'Jopa'  <joel@jopa.fr>
 *
 * This file is part of the pam_mir-ror project. pam_mir-ror is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * pam_mir-ror is distributed in the hope that it will be useful, but WITHOUT ANY
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

#include "mir-ror.h"

#include "version.h"

#define PAM_SM_AUTH

// thread_pam_prompt : prompt user to use mir:ror
void * thread_pam_prompt(void* data) {
    pam_handle_t *pamh = data;
    char *resp;
    pam_prompt(pamh,PAM_PROMPT_ECHO_OFF, &resp,"Ok ! Show me your Ztamp:s : ");

    return NULL;
}

//----------------------------------------------------------------------------
// pam_sm_authenticate : pam function - user authentification
PAM_EXTERN
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    int retval;
    const char *service;
    const char *user;
    const char *tty;

    char device[50];
    
    char stored_tag[25];
    char mirror_tag[25];    

    char tag_file_path[256];
    FILE *tagfile;

    int term_isatty;
    struct termios term_attr;

    pthread_t prompt;

    openlog ("[pam_mirror]", LOG_PID, LOG_AUTH);

    term_isatty = isatty(STDIN_FILENO);

    //Save term Attributes
    if (term_isatty==1) {
       tcgetattr(STDIN_FILENO,&term_attr);
    }
    
    //Get service
    retval = pam_get_item(pamh, PAM_SERVICE, (const void **)(const void *)&service);

    if (retval != PAM_SUCCESS) {
      syslog(LOG_WARNING, "Unable to retrieve the PAM service name.\n");
      return (PAM_AUTH_ERR);
      closelog();
    }

    //Get user
    if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS || !user || !*user) {
        syslog(LOG_WARNING, "Unable to retrieve the PAM user name.\n");
	closelog();
	return (PAM_AUTH_ERR);
    }

    syslog(LOG_WARNING,"Authentification request for user '%s' (%s)\n",user,service);

    //No rfid use for ssh tty
    if (pam_get_item(pamh, PAM_TTY, (const void **)(const void *)&tty) == PAM_SUCCESS) {
       if (tty && !strcmp(tty,"ssh")) {
         syslog(LOG_WARNING,"Not using RFID for SSH Authentification.\n");
         closelog();
	 return (PAM_AUTH_ERR); 
       }
    }

    //User .authtag file
    get_user_tagfile((char *)user,tag_file_path);
    tagfile = fopen (tag_file_path,"r");
    
    if (tagfile==NULL) {
      syslog(LOG_WARNING,"Unable to open rfid tag file : %s, for user : %s ",tag_file_path,user);
      closelog();
      return (PAM_SERVICE_ERR);
    }	
    
    fgets(stored_tag,25,tagfile);
    
    // Detect mir:ror device
    if (detect_mirror(device)!=0) {
      syslog(LOG_WARNING, "Unable to detect Mir:ror device.\n");
      closelog();
      return (PAM_AUTH_ERR);
    }

    // Create thread for prompt display    
    pthread_create (&prompt,NULL,thread_pam_prompt,pamh);

    if (get_mirror_tag(mirror_tag,device)!=0) {
      syslog(LOG_WARNING, "Failed to access Mir:ror device : %s",device);
      closelog();
      return(PAM_AUTH_ERR);
    }
    
    // Ok, killing prompt thread now...
    pthread_cancel(prompt);   
    
    //Restore term attributes
    if (term_isatty==1) {
       tcsetattr(STDIN_FILENO,TCSADRAIN,&term_attr);
    }
    printf("\n");

    // Compare stored tag with ztamp:s tag
    if (strcmp (stored_tag,mirror_tag)==0) {
	syslog(LOG_WARNING,"Authentification granted for user '%s' (%s)\n",user,service);
    	closelog();
	return (PAM_SUCCESS);
    
    } else {
	syslog(LOG_WARNING,"Authentification failure for user '%s' (%s)\n",user,service);
	closelog();
	
	return (PAM_AUTH_ERR);
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
struct pam_module _pam_mirror_modstruct = {
   	"pam_mirror",
   	pam_sm_authenticate,
	pam_sm_setcred,
	NULL,
	NULL,
	NULL,
	pam_sm_chauthtok
};

#endif

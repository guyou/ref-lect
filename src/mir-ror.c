// mir-ror.c
/*
 * Copyright (c) 2009 - Joël PASTRE 'Jopa'  <joel@jopa.fr> - http://www.jopa.fr
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

#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/hiddev.h>
#include <errno.h>
#include <string.h>

#define HIDIOCGRAWINFO          _IOR('H', 0x03, struct hidraw_devinfo)

struct hidraw_devinfo {
        __u32 bustype;
         __s16 vendor;
         __s16 product;
};

int check_fd_device(int fd) {
	struct hidraw_devinfo hrdi;
	ioctl(fd,HIDIOCGRAWINFO,&hrdi);
	return hrdi.vendor == 0x1da8 && hrdi.product == 0x1301;
}

int check_device(char * device) {
	int fd;
	int exit_value = 0;
	
	if((fd=open(device, O_RDONLY)) < 0) {
		int errsv = errno;
		fprintf(stderr, "Error while opening %s: %s.\n", device, strerror(errsv));
		return 0;
	}
	if (check_fd_device(fd)) {
		exit_value = 1;
	}
	close(fd);

	return exit_value;
}

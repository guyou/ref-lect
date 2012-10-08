# $Id: Makefile.am 13 2003-07-31 12:40:54Z lennart $
#
# This file is part of ref:lect.
#
# ref:lect is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# ref:lect is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with pam_dbus; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

AM_CFLAGS = -DSBINDIR=\"@sbindir@\" @PAM_REF_LECT_CFLAGS@

pamdir = @PAM_MODDIR@
pam_LTLIBRARIES = pam_reflect.la

pam_reflect_la_SOURCES = pam_ref-lect.c mirror.h mirror.c
pam_reflect_la_LDFLAGS = -module -avoid-version
pam_reflect_la_LIBADD = @PAM_REF_LECT_LIBS@
pam_reflect_la_CFLAGS = $(AM_CFLAGS)

check_PROGRAMS = check_token wait_token

check_token_SOURCES = check_token.c mirror.h mirror.c
check_token_LDADD = @PAM_REF_LECT_LIBS@
check_token_CFLAGS = $(AM_CFLAGS)

wait_token_SOURCES = wait_token.c mirror.h mirror.c
wait_token_LDADD = @PAM_REF_LECT_LIBS@
wait_token_CFLAGS = $(AM_CFLAGS)

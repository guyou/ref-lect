# $Id: Makefile.am 13 2003-07-31 12:40:54Z lennart $
#
# This file is part of ref-lect.
#
# pam_dbus is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# pam_dbus is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with pam_dbus; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

AM_CFLAGS = -DSBINDIR=\"@sbindir@\" @PAM_MIRROR_CFLAGS@

pamdir = @PAM_MODDIR@
pam_LTLIBRARIES = pam_mirror.la

pam_mirror_la_SOURCES = pam_mir-ror.c check_token.h check_token.c
pam_mirror_la_LDFLAGS = -module -avoid-version
pam_mirror_la_LIBADD = @PAM_MIRROR_LIBS@
pam_mirror_la_CFLAGS = $(AM_CFLAGS)

check_PROGRAMS = test_check_token

test_check_token_SOURCES = test_check_token.c check_token.h check_token.c
test_check_token_LDADD = @PAM_MIRROR_LIBS@
test_check_token_CFLAGS = $(AM_CFLAGS)

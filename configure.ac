dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(ref-lect, 0.3)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CC


LT_INIT

GETTEXT_PACKAGE=gitg
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package])

dnl Check for vala
AM_PROG_VALAC([0.10.0])


PKG_CHECK_MODULES(REF_LECT, [
	gio-2.0
	gee-1.0
	dbus-1
])


AC_OUTPUT([
Makefile
src/Makefile
data/Makefile
tools/Makefile

])

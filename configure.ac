dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(ref-lect, 0.4)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_LIBTOOL

LT_INIT

GETTEXT_PACKAGE=gitg
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package])

dnl Check for vala
AM_PROG_VALAC([0.10.0])

dnl Checks for libraries.
AC_CHECK_HEADER([security/pam_modules.h],, [AC_MSG_ERROR([*** Sorry, you have to install the PAM development files ***])])

LIBS="$LIBS -ldl -lpam -lpam_misc"

case "$host" in
  *-*-linux*)
    PAM_MODDIR="${libdir}/security"
    ;;
  *)
    PAM_MODDIR="${libdir}"
    ;;
esac
AC_SUBST(PAM_MODDIR)


PKG_CHECK_MODULES(REF_LECT, [
	glib-2.0
	gio-2.0
	gee-1.0
	dbus-1
])

PKG_CHECK_MODULES(PAM_REF_LECT, [
	glib-2.0
	dbus-1
	dbus-glib-1
])

iconsdir=$datadir/reflect/icons
AC_SUBST([iconsdir], [$iconsdir])


AC_OUTPUT([
Makefile
src/Makefile
data/Makefile
erawrim/Makefile
erawrim/examples/Makefile
pam_ref-lect/Makefile
tools/Makefile

])

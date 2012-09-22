## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(REF_LECT_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = reflectd

reflectd_SOURCES = \
	EventType.vala EventTypeParser.vala \
	reflectd.vala config.vapi MirrorDevice.vala mir-ror.c mir-ror.vapi

reflectd_VALAFLAGS =  \
	--pkg gio-2.0 --pkg gee-1.0 --pkg posix

reflectd_LDFLAGS = \
	-Wl,--export-dynamic

reflectd_LDADD = $(REF_LECT_LIBS)



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

bin_PROGRAMS = ref-lect

ref_lect_SOURCES = \
	ref-lect.vala config.vapi MirrorDevice.vala mir-ror.c mir-ror.vapi

ref_lect_VALAFLAGS =  \
	--pkg gio-2.0 --pkg posix

ref_lect_LDFLAGS = \
	-Wl,--export-dynamic

ref_lect_LDADD = $(REF_LECT_LIBS)



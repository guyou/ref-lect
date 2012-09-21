## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src data erawrim pam_mir-ror tools

ref_lectdocdir = ${prefix}/doc/ref-lect
ref_lectdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(ref_lectdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(ref_lectdocdir)

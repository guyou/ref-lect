ref_lectexamplesdir = ${prefix}/doc/ref-lect/examples
ref_lectexamples_DATA = \
	pose.sh \
	retire.sh \
	envers.sh \
	endroit.sh

iconsdir = $(datadir)/ref-lect/icons

envers.sh: $(srcdir)/envers.sh.in
	$(AM_V_GEN)sed -e 's,@ICONSDIR\@,$(iconsdir),g' < $< > $@

endroit.sh: $(srcdir)/endroit.sh.in
	$(AM_V_GEN)sed -e 's,@ICONSDIR\@,$(iconsdir),g' < $< > $@

pose.sh: $(srcdir)/pose.sh.in
	$(AM_V_GEN)sed -e 's,@ICONSDIR\@,$(iconsdir),g' < $< > $@

retire.sh: $(srcdir)/retire.sh.in
	$(AM_V_GEN)sed -e 's,@ICONSDIR\@,$(iconsdir),g' < $< > $@

EXTRA_DIST = \
	pose.sh.in \
	retire.sh.in \
	envers.sh.in \
	endroit.sh.in

DISTCLEANFILES = \
	$(ref_lectexamples_DATA)

ref_lectexamplesdir = ${prefix}/doc/ref-lect/examples
ref_lectexamples_DATA = \
	envers.sh \
	endroit.sh

iconsdir = $(datadir)/ref-lect/icons

envers.sh: $(srcdir)/envers.sh.in
	$(AM_V_GEN)sed -e 's,@ICONSDIR\@,$(iconsdir),g' < $< > $@

endroit.sh: $(srcdir)/endroit.sh.in
	$(AM_V_GEN)sed -e 's,@ICONSDIR\@,$(iconsdir),g' < $< > $@

EXTRA_DIST = \
	envers.sh.in \
	endroit.sh.in

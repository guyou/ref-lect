man_MANS = \
	ref-lect.1 \
	erawrim.1 \
	erawrim-ctl.1

dbussystemdir = $(sysconfdir)/dbus-1/system.d/
dbusservicedir = $(datadir)/dbus-1/system-services/
dbussystem_DATA = \
	org.rfid.Mirror.conf
dbusservice_DATA = \
	org.rfid.Mirror.service

org.rfid.Mirror.service: $(srcdir)/org.rfid.Mirror.service.in
	$(AM_V_GEN)sed -e 's,@BINDIR\@,$(bindir),g' < $< > $@

EXTRA_DIST = \
	$(man_MANS) \
	$(dbussystem_DATA) \
	org.rfid.Mirror.service.in

DISTCLEANFILES = \
	org.rfid.Mirror.service

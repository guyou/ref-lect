man_MANS = \
	reflectd.1 \
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

iconsdir = $(datadir)/ref-lect/icons
icons_DATA = reflect_logo.png

EXTRA_DIST = \
	$(man_MANS) \
	$(dbussystem_DATA) \
	reflect_logo.xcf \
	$(icons_DATA) \
	org.rfid.Mirror.service.in

DISTCLEANFILES = \
	org.rfid.Mirror.service

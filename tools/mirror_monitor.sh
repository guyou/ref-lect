#!/bin/sh

# Output example
# signal sender=:1.19 -> dest=(null destination) serial=3 path=/org/rfid/mirror; interface=org.rfid.Mirror; member=TagEnter
#    string "08D0021A0352C102B000"
# signal sender=:1.19 -> dest=(null destination) serial=4 path=/org/rfid/mirror; interface=org.rfid.Mirror; member=TagLeave
#    string "08D0021A0352C102B000"
# signal sender=:1.19 -> dest=(null destination) serial=5 path=/org/rfid/mirror; interface=org.rfid.Mirror; member=FlipDown
# signal sender=:1.19 -> dest=(null destination) serial=6 path=/org/rfid/mirror; interface=org.rfid.Mirror; member=FlipUp
#
# See also: http://rg42.org/oss/dbustriggerd/start

dbus-monitor "type='signal',sender='org.rfid.Mirror',interface='org.rfid.Mirror'"

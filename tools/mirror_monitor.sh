#!/bin/sh

# Output example
# signal sender=:1.19 -> dest=(null destination) serial=3 path=/org/rfid/mirror; interface=org.rfid.Mirror; member=tagEnter
#    string "08D0021A0352C102B000"
# signal sender=:1.19 -> dest=(null destination) serial=4 path=/org/rfid/mirror; interface=org.rfid.Mirror; member=tagLeave
#    string "08D0021A0352C102B000"
# signal sender=:1.19 -> dest=(null destination) serial=5 path=/org/rfid/mirror; interface=org.rfid.Mirror; member=flipDown
# signal sender=:1.19 -> dest=(null destination) serial=6 path=/org/rfid/mirror; interface=org.rfid.Mirror; member=flipUp
#
# See also: http://rg42.org/oss/dbustriggerd/start

dbus-monitor "type='signal',sender='org.rfid.Mirror',interface='org.rfid.Mirror'"

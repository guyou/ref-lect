#!/bin/sh

#BUS=--session
BUS=--system
# Get id of the mirror
#
# Example result:
# method return sender=:1.5 -> dest=:1.14 reply_serial=2
#   string "UP"
dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetId

# Get state of the mirror
#
# Example result:
# method return sender=:1.5 -> dest=:1.14 reply_serial=2
#   string "UP"
dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetState

# Get list of tags on the mirror
#
# Example result:
# method return sender=:1.5 -> dest=:1.17 reply_serial=2
#    array [
#       string "D0021A0352C102B0"
#       string "D0021A0352D003E1"
#    ]
dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetTags

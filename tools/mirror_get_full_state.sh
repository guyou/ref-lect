#!/bin/sh

#BUS=--session
BUS=--system
# Get the id of the Mir:ror
echo "Mir:ror's id:"
dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetId

# Get the Mir:ror's application version
echo "Mir:ror's application version:"
dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetAppVersion

# Get the Mir:ror's bootloader version
echo "Mir:ror's bootloader version:"
dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetBootVersion

# Get state of the mirror
#
# Example result:
# method return sender=:1.5 -> dest=:1.14 reply_serial=2
#   string "UP"
echo "Mir:ror's state:"
dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetState

# Get list of tags on the mirror
#
# Example result:
# method return sender=:1.5 -> dest=:1.17 reply_serial=2
#    array [
#       string "D0021A0352C102B0"
#       string "D0021A0352D003E1"
#    ]
echo "Ztamps on the Mir:ror:"
dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetTags

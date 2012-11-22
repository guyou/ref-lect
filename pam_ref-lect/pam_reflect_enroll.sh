#!/bin/bash
BUS=--system
FILE=~/.authtag

tags=`dbus-send $BUS --print-reply --dest=org.rfid.Mirror /org/rfid/mirror org.rfid.Mirror.GetTags | awk '/string/{print $2}' | tr -d '"'`

select tag in $tags quit
do
  if [ "$tag" != "quit" ]
  then
    echo -n "$tag" > $FILE
    chmod og-rwx $FILE
  fi
  exit 0
done

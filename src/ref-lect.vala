/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) 2012 Guilhem Bonnefille <guilhem.bonnefille@gmail.com>
 * 
 * ref-lect is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ref-lect is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

using GLib;

[DBus (name = "org.rfid.Mirror")]
public class MirrorServer : Object {

    private uint16 state = Event.UP;
	private string[] tagList;	

    public string getState () {
        switch(state)
		{
			case Event.UP:
				return "UP";
			case Event.DOWN:
				return "DOWN";
			default:
				return "UNKNOWN";
		}
    }

    public string[] getTags () {
        return tagList;
    }

    public signal void tagEnter (string tag);
    public signal void tagLeave (string tag);
    public signal void flipUp ();
    public signal void flipDown ();

	[DBus (visible = false)]
	public void processEvent (uint16 event, ref string tag) {
		switch(event)
		{
			case Event.UP:
				stdout.printf ("DEBUG: mirror flipped up\n");
				state = Event.UP;
				this.flipUp ();
				break;
			case Event.DOWN:
				stdout.printf ("DEBUG: mirror flipped down\n");
				state = Event.DOWN;
				this.flipDown ();
				break;
			case Event.ENTER:
				stdout.printf ("DEBUG: tag entered: %s\n", tag);
				tagList += tag;
				this.tagEnter (tag);
				break;
			case Event.LEAVE:
				stdout.printf ("DEBUG: tag left: %s\n", tag);
				// tagList.remove (tag);
				this.tagLeave (tag);
				break;
			default:
				break;
		}
	}
}

MirrorServer mirror;

void on_bus_aquired (DBusConnection conn) {
    try {
        conn.register_object ("/org/rfid/mirror", mirror);
    } catch (IOError e) {
        stderr.printf ("Could not register service\n");
    }
}

async void read_async (MirrorDevice dev)
{
    try {
		while (true)
		{
			// FIXME break loop cleanly
			string tag = "";
			uint16 event = yield dev.read_event (out tag);
			if (event != Event.EMPTY)
			stdout.printf ("DEBUG: event: %d %04X\n", event, event);
			mirror.processEvent (event, ref tag);
		}
    } catch (Error e) {
        error (e.message);
    }
}

public class Main : Object 
{

	static int main (string[] args) 
	{
		mirror = new MirrorServer ();
	    Bus.own_name (BusType.SESSION, "org.rfid.Mirror", BusNameOwnerFlags.NONE,
              on_bus_aquired,
              () => stdout.printf ("DEBUG: Name aquired\n"),
              () => stderr.printf ("Could not aquire name\n"));

		string device = MirrorDevice.detect_mirror ();
		stdout.printf ("DEBUG: Device found %s\n", device);

		if (device != null)
		{
			var dev = new MirrorDevice (device);
			var loop = new MainLoop();
			read_async(dev);
			loop.run();
			return 0;
		}
		else
			return 1;
	}
}

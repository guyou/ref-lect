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
using Gee;
using Mirror;

[DBus (name = "org.rfid.Mirror")]
public class MirrorServer : Object {

	public string id = "<not-set>";
	public string app_version = "<not-set>";
	public string boot_version = "<not-set>";
	private uint16 state = EventType.OrientationUp;
	private HashSet<string> tagList = new HashSet<string> ();	

	public string get_id () {
		return id;
	}

	public string get_app_version () {
		return app_version;
	}
	
	public string get_boot_version () {
		return boot_version;
	}
	
	public string get_state () {
        switch(state)
		{
			case EventType.OrientationUp:
				return "UP";
			case EventType.OrientationDown:
				return "DOWN";
			default:
				return "UNKNOWN";
		}
    }

    public string[] get_tags () {
        return tagList.to_array ();
    }

    public void reset_all () {
        tagList.clear ();
    }

    public signal void tag_enter (string tag);
    public signal void tag_leave (string tag);
    public signal void flip_up ();
    public signal void flip_down ();

	[DBus (visible = false)]
	public void processEvent (EventType event, ref string tag) {
		switch(event)
		{
			case EventType.Unspecified:
				break;
			case EventType.OrientationUp:
				stdout.printf ("DEBUG: mirror flipped up\n");
				state = event;
				this.flip_up ();
				break;
			case EventType.OrientationDown:
				stdout.printf ("DEBUG: mirror flipped down\n");
				state = event;
				this.flip_down ();
				break;
			case EventType.ShowTag:
				stdout.printf ("DEBUG: tag entered: %s\n", tag);
				tagList.add (tag);
				this.tag_enter (tag);
				break;
			case EventType.HideTag:
				stdout.printf ("DEBUG: tag left: %s\n", tag);
				tagList.remove (tag);
				this.tag_leave (tag);
				break;
			case EventType.MirrorId:
				stdout.printf ("DEBUG: Mir:ror id: %s\n", tag);
				this.id = tag;
				break;
			case EventType.ApplicationVersion:
				stdout.printf ("DEBUG: Mir:ror application's version: %s\n", tag);
				this.app_version = tag;
				break;
			case EventType.BootloaderVersion:
				stdout.printf ("DEBUG: Mir:ror bootloader's version: %s\n", tag);
				this.boot_version = tag;
				break;
			default:
				stdout.printf ("DEBUG: event %s payload %s\n", event.to_string(), tag);
				break;
		}
	}
}

MirrorServer mirror;

void on_bus_aquired (DBusConnection conn) {
    try {
        conn.register_object ("/org/rfid/mirror", mirror);
    } catch (IOError e) {
        stderr.printf ("Could not register service: %s\n", e.message);
    }
}

async void read_async (MirrorDevice dev)
{
    try {
		while (true)
		{
			// FIXME break loop cleanly
			string tag = "";
			EventType event = yield dev.read_event (out tag);
			if (event != EventType.Unspecified)
			stdout.printf ("DEBUG: event: %d %04X\n", event, event);
			mirror.processEvent (event, ref tag);
		}
    } catch (Error e) {
        error (e.message);
    }
}

public class Main : Object 
{
	public static bool quit = false;
	public static bool session = false;
	public static bool system = false;

	public static const OptionEntry[] entries = {
		{"version", 'v', OptionFlags.NO_ARG, OptionArg.CALLBACK,
			(void *)show_version_and_quit, N_("Show the application's version"), null},
		{"system", 0, 0, OptionArg.NONE,
			ref system, N_("Use system message bus."), null},
		{"session", 0, 0, OptionArg.NONE,
			ref session, N_("Use session message bus."), null},
		{null}
	};

	private static void show_version_and_quit()
	{
		stdout.printf("%s %s\n",
		              Environment.get_application_name(),
		              Config.VERSION);

		quit = true;
	}

	private static void parse_command_line(ref unowned string[] argv) throws OptionError
	{
		var ctx = new OptionContext(_("- Mir:ror userspace driver"));

		ctx.add_main_entries(entries, Config.GETTEXT_PACKAGE);

		ctx.parse(ref argv);
	}


	static int main (string[] args) 
	{
	try {
		parse_command_line(ref args);

		if (quit)
			return 0;
		
		mirror = new MirrorServer ();
		BusType busType;
		if (session && !system)
			busType = BusType.SESSION;
		else if (!session && system)
			busType = BusType.SYSTEM;
		else
			busType = BusType.SYSTEM;
		Bus.own_name (busType, "org.rfid.Mirror", BusNameOwnerFlags.NONE,
		              on_bus_aquired,
		              () => stdout.printf ("DEBUG: Name aquired\n"),
		              () => stderr.printf ("Could not aquire name\n"));

		string device = MirrorDevice.detect_mirror ();
		stdout.printf ("DEBUG: Device found %s\n", device);

		if (device != null)
		{
			var dev = new MirrorDevice (device);
			// Send commands to force events
			// This will allow to update the internal state
			dev.write_event (EventType.GetMirrorId);
			dev.write_event (EventType.GetOrientation);
			dev.write_event (EventType.GetApplicationVersion);
			dev.write_event (EventType.GetBootloaderVersion);
			var loop = new MainLoop();
			read_async(dev);
			loop.run();
			return 0;
		}
		else
			return 1;
	} catch (GLib.OptionError e) {
		error(e.message);
	} catch (GLib.Error e) {
		error(e.message);
	}
	}
}

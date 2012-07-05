/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
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

using Posix;
using GLib;
using Mirror;

public class MirrorDevice : Object 
{


	private string devname;
	private FileStream device;
	
	public static string detect_mirror ()
	{
		int i;
		string devname = null;
		var devices = Glob();

		int ret = devices.glob("/dev/hidraw*", 0);
		if (ret != 0)
		{
			GLib.stderr.printf("glob() failed.\n");
		}

		GLib.stdout.printf("DEBUG: found %i entries\n", (int)devices.pathc);
		for(i=0;i<devices.pathc;i++) {
			GLib.stdout.printf("DEBUG: found %s\n", devices.pathv[i]);
			if(check_device(devices.pathv[i]) != 0) {
				// Found
				devname = devices.pathv[i];
			}
		}

		return devname;
	}

	public MirrorDevice (string devname) throws Error
	{
		this.devname = devname;
		device = FileStream.open (devname, "rb");
	}

	public async EventType read_event (out string tag) throws IOError
	{
		uint8[] event_bytes = new uint8[2];
		device.read (event_bytes);
		if (event_bytes[0] != 0 || event_bytes[1] != 0)
			GLib.stdout.printf("DEBUG: Event %02X %02X\n", event_bytes[0], event_bytes[1]);
		
		EventType event = EventTypeParser.Parse (event_bytes);

		var buf = new StringBuilder();
		if(event != EventType.Unspecified)
		{
			// Skip 2 bytes
			device.getc ();
			device.getc ();
			// 1 byte: length of payload
			int len = device.getc ();
			GLib.stdout.printf("DEBUG: Len %d\n", len);
			// Read payload
			uint8[] tag_bytes = new uint8[len];
			device.read (tag_bytes);
			foreach (uint8 cur in tag_bytes)
			{
				buf.append("%02X".printf(cur));
			}
			// Align
			// Read all next bytes to align
			// => 16 bytes less all already read bytes
			for (int i = 0 ; i < (16 - (2+2+1+len)) ; i++)
			{
				device.getc ();
			}
		}
		tag = buf.str; 

		return event;
	}
}

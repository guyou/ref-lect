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

using Posix;
using GLib;

enum Event {
	EMPTY = 0x000,
	UP = 0x401,
	DOWN = 0x501,
	ENTER = 0x102,
	LEAVE = 0x202
}

public class MirrorDevice : Object 
{


	private string device;
	private DataInputStream dis;
	
	public static string detect_mirror ()
	{
		int i;
		string device = null;
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
				device = devices.pathv[i];
			}
		}

		return device;
	}

	public MirrorDevice (string device) throws Error
	{
		this.device = device;
		var file = File.new_for_path (device);
		dis = new DataInputStream (file.read ());
		dis.set_byte_order (DataStreamByteOrder.LITTLE_ENDIAN);
	}

	public async uint16 read_event (out string tag) throws IOError
	{
		uint8[] event_bytes = new uint8[2];
		dis.read (event_bytes);
		// FIXME read_uint16 corrupt next reads
		uint16 event = (event_bytes[1] << 8) + event_bytes[0];

		var buf = new StringBuilder();
		if(event != Event.EMPTY)
		{
			dis.skip (3);
			uint8[] tag_bytes = new uint8[2*4];
			dis.read (tag_bytes);
			foreach (uint8 cur in tag_bytes)
			{
				buf.append("%02X".printf(cur));
			}
			dis.skip (3);
		}
		tag = buf.str; 

		return event;
	}
}
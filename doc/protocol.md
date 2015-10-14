---
title: "Mir:ror communication protocol"
---

Messages
===

Generic format
---

 Meaning     |  Size
:-----------:|:------:
Interface ID | 1 byte
Method ID    | 1 byte
Msg Id       | 2 bytes
Len          | 1 byte
Data         | len * byte(s)

When device has to reply to a command, Msg Id is copied from request.
When device has to send message due to external event (flip, tag...), then Msg Id is set to zero.

References
===

* <http://luotio.blogspot.fr/2010/08/using-libmirror-to-read-rfid-tags-with.html>
* <http://blog.nomzit.com/2010/01/30/decoding-the-mirror-comms-protocol/>

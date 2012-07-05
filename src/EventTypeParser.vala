namespace Mirror
{
    public class EventTypeParser
    {
        public static EventType Parse( uint8[] eventTypeBytes )
        {
            uint16 cmd = (eventTypeBytes[0] << 8) | eventTypeBytes[1];

            return (EventType) cmd;
        }
    }
}

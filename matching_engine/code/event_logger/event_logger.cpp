#include "event_logger.h"

#include <iostream>

#include "globals.h"
#include "event_receiver_id.h"

using namespace std;

void EventLogger_EventHandler(Event event)
{
    cout << "event_logger: event_id=" << event.GetEventId() << ", data=" << event.GetJsonData() << endl;
}

void EventLogger_Subscribe()
{
    int event_id;

    for(event_id = 0; event_id < EVENT_ID_INVALID; event_id++)
    {
        event_bus.Subscribe(RECEIVER_ID_EVENT_LOGGER, event_id);
    }
}
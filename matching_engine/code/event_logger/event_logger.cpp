#include "event_logger.h"


#include "event_logger_event_receiver.h"

#include <iostream>

EventLogger::EventLogger() {}
EventLogger::~EventLogger() {}

ReturnType EventLogger::ConvertEventIdToString(enum eEventId_t event_id, string& event_id_s)
{
    ReturnType ret = RET_NOT_OK;

    if(event_id <= EVENT_ID_INVALID)
    {
        event_id_s = this->eventIdToStringMap[event_id];

        ret = RET_OK;
    }

    return ret;
}

void EventLogger::EventHandler(Event event)
{
    string event_id_s;

    if(RET_OK == this->ConvertEventIdToString(event.GetEventId(), event_id_s))
    {
        cout << "event_logger: event_id=" << event_id_s << ", data=" << event.GetJsonData() << endl;
    }
}

void EventLogger::init(EventBus& event_bus)
{
    int event_id;

    event_bus.AddReceiver(EventLogger_EventReceiver(RECEIVER_ID_EVENT_LOGGER, *this));

    for(event_id = 0; event_id < EVENT_ID_INVALID; event_id++)
    {
        event_bus.Subscribe(RECEIVER_ID_EVENT_LOGGER, static_cast<enum eEventId_t>(event_id));
    }
}
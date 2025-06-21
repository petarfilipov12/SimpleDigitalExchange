#include "event_logger.h"

#include <iostream>

EventLogger::EventLogger() {}
EventLogger::~EventLogger() {}

returnType EventLogger::ConvertEventIdToString(eventId_t event_id, std::string& event_id_s)
{
    returnType ret = RET_NOT_OK;

    if(event_id <= EVENT_ID_INVALID)
    {
        event_id_s = this->eventIdToStringMap[event_id];

        ret = RET_OK;
    }

    return ret;
}

void EventLogger::EventHandler(Event event)
{
    std::string event_id_s;

    if(RET_OK == this->ConvertEventIdToString(event.GetEventId(), event_id_s))
    {
        std::cout << "event_logger: event_id=" << event_id_s << ", data=" << event.GetJsonData() << std::endl;
    }
}

void EventLogger::init(EventBus& event_bus)
{
    int event_id;

    EventReceiver event_receiver = EventReceiver(
        RECEIVER_ID_EVENT_LOGGER, 
        std::bind(&EventLogger::EventHandler, this, std::placeholders::_1),
        nullptr
    );

    event_bus.AddReceiver(event_receiver);

    for(event_id = 0; event_id < EVENT_ID_INVALID; event_id++)
    {
        event_bus.Subscribe(RECEIVER_ID_EVENT_LOGGER, static_cast<eventId_t>(event_id));
    }
}
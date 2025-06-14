#ifndef EVENT_LOGGER_EVENT_RECEIVER_H
#define EVENT_LOGGER_EVENT_RECEIVER_H

#include "types.h"

#include "event_receiver.h"
#include "event_logger.h"
#include "event.h"

#include <iostream>

class EventLogger_EventReceiver: public EventReceiver
{
    private:
        EventLogger& event_logger;

    public:
        EventLogger_EventReceiver(const enum eReceiverId_t id, EventLogger& event_loggerP): EventReceiver(id), event_logger(event_loggerP) {}

        void CallEventHandler(const Event& event)
        {
            cout << "EventLogger_EventReceiver.CallEventHandler()\n";
            this->event_logger.EventHandler(event);
        }
};

#endif
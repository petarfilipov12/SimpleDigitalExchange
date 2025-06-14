#ifndef ENGINE_EVENT_RECEIVER_H
#define ENGINE_EVENT_RECEIVER_H

#include "types.h"

#include "event_receiver.h"
#include "engine.h"
#include "event.h"

#include <iostream>

class Engine_EventReceiver: public EventReceiver
{
    private:
        Engine& engine;

    public:
        Engine_EventReceiver(const enum eReceiverId_t id, Engine& engineP): EventReceiver(id), engine(engineP) {}

        void CallEventHandler(const Event& event)
        {
            cout << "Engine_EventReceiver.CallEventHandler()\n";
            this->engine.EventHandler(event);
        }
};

#endif
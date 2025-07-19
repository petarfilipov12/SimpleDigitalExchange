#include "cache.h"

Cache::Cache(const std::string& symbol, const EventReceiver& event_receiver)
{
    this->symbol = symbol;
    this->event_receiver = event_receiver;
}

Cache::~Cache() {}

void Cache::init(EventBus &event_bus, const std::vector<eventId_t> &events_to_subscribe)
{
    receiverId_t receiver_id = this->event_receiver.GetId();

    event_bus.AddReceiver(&this->event_receiver);

    for (int i = 0; i < events_to_subscribe.size(); i++)
    {
        event_bus.Subscribe(receiver_id, events_to_subscribe.at(i));
    }
}

returnType Cache::Filter(Event &event)
{
    returnType ret = RET_NOT_OK;

    if (this->symbol == event.GetDataIn()["symbol"])
    {
        ret = RET_OK;
    }

    return ret;
}
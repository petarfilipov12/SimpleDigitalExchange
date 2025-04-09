#ifndef EVENT_H
#define EVENT_H

#include<nlohmann/json.hpp>
using json = nlohmann::json;

#include "event_ids.h"

typedef struct Event{
    int event_id;
    json json_data;
}Event_t;

#endif
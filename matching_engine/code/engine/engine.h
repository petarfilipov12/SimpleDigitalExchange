#ifndef ENGINE_H
#define ENGINE_H

#include "types.h"
#include "order_book.h"
#include "taker_order_book.h"
#include "order.h"
#include "event.h"
#include "event_bus.h"

#include<nlohmann/json.hpp>
using json = nlohmann::json;

class Engine{
    private:
        OrderBook order_book;
        TakerOrderBook taker_book;
        EventBus& event_bus;

        ReturnType AddToOrderBook(Order& pTakerOrder);
        
        ReturnType MatchTakerOrder(Order& pTakerOrder);
        
        void Cyclic();

        void EventHandler_AddOrder(Event& event);

        void EventHandler_CancelOrder(Event& event);
    
    public:
        Engine(EventBus& event_busP);
        
        ~Engine();
        
        bool ExistsOrder(const Order& order) const;
        
        bool ExistsOrderId(const int id) const;
        
        ReturnType AddOrder(Order order);
        
        ReturnType CancelOrder(const Order& order);
        
        ReturnType CancelOrderById(const int id);
        
        void run();

        void EventHandler(Event event);

        void init();
};

#endif
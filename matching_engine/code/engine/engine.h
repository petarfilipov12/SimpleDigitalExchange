#ifndef ENGINE_H
#define ENGINE_H

#include "return_type.h"
#include "order_book.h"
#include "taker_order_book.h"
#include "order.h"
#include "event.h"
#include "event_bus.h"

#include "json.h"


class Engine{
    private:
        OrderBook order_book;
        TakerOrderBook taker_book;
        EventBus& event_bus;
        std::string symbol;

        returnType AddToOrderBook(Order& pTakerOrder);
        
        returnType MatchTakerOrder(Order& pTakerOrder);

        bool ExistsOrder(const Order& order) const;
        
        bool ExistsOrderId(const int id) const;
        
        returnType AddOrder(Order order);
        
        returnType CancelOrder(const Order& order);
        
        returnType CancelOrderById(const int id);
        
        void Cyclic();

        void run();

        void EventHandler_AddOrder(Event& event);

        void EventHandler_CancelOrder(Event& event);

        void EventHandler(Event event);

        returnType Filter(Event& event);
    
    public:
        Engine(const std::string& symbol, EventBus& event_bus, receiverId_t receiver_id);
        
        ~Engine();
};

#endif
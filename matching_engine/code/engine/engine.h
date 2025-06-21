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
        
        void Cyclic();

        void EventHandler_AddOrder(Event& event);

        void EventHandler_CancelOrder(Event& event);
    
    public:
        Engine(EventBus& event_busP, const std::string& symbol);
        
        ~Engine();
        
        bool ExistsOrder(const Order& order) const;
        
        bool ExistsOrderId(const int id) const;
        
        returnType AddOrder(Order order);
        
        returnType CancelOrder(const Order& order);
        
        returnType CancelOrderById(const int id);
        
        void run();

        void init(receiverId_t receiver_id);

        void EventHandler(Event event);

        returnType Filter(Event& event);
};

#endif
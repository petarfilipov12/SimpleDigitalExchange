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
        EventBus *event_bus;
    
    public:
        Engine(EventBus *event_bus);
        
        ~Engine();
        
        bool ExistsOrder(Order order) const;
        
        bool ExistsOrderId(int id) const;
        
        Return_Type AddOrder(Order order);
        
        Return_Type CancelOrder(Order order);
        
        Return_Type CancelOrderById(int id);

        Return_Type AddToOrderBook(Order *pTakerOrder);
        
        Return_Type MatchTakerOrder(Order *pTakerOrder);
        
        void Cyclic();
        
        void run();
};

#endif
#ifndef ENGINE_H
#define ENGINE_H

#include "types.h"
#include "order_book.h"
#include "market_order_book.h"
#include "order.h"
#include "event.h"
#include "event_bus.h"

#include<nlohmann/json.hpp>
using json = nlohmann::json;

class Engine{
    private:
        OrderBook order_book;
        MarketOrderBook market_book;
        EventBus *event_bus;
    
    public:
        Engine(EventBus *event_bus);
        
        ~Engine();
        
        bool ExistsOrder(Order order) const;
        
        bool ExistsOrderId(int id) const;
        
        Return_Type AddOrder(Order order);
        
        Return_Type CancelOrder(Order order);
        
        Return_Type CancelOrderById(int id);
        
        Return_Type GetOrderBook(json *l2_book);

        void PrintOrderBook();
        
        void MatchOrderBook();
        
        void MatchMarketOrder();
        
        void Cyclic();
        
        void run();
};

#endif
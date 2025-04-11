#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>

#include "order_book.h"
#include "market_order_book.h"
#include "order.h"
#include "event.h"
#include "event_bus.h"

using namespace std;

class Engine{
    private:
        OrderBook order_book;
        MarketOrderBook market_book;
        EventBus *event_bus;
    
    public:
        Engine(EventBus *event_bus)
        {
            this->event_bus = event_bus;
        }
        
        ~Engine()
        {
            // this->market_orders_queue.clear();
            // this->market_orders.clear();
        
            // this->order_book.~OrderBook();
        }
        
        bool ExistsOrder(Order order) const
        {
            bool ret = false;
        
            if (order.order_type == ORDER_TYPE_MARKET)
            {
                ret = this->market_book.ExistsMarketOrder(order);
            }
            else if (order.order_type == ORDER_TYPE_LIMIT)
            {
                ret = this->order_book.ExistsOrder(order);
            }
            else
            {
                // Nothing to do
            }
        
            return ret;
        }
        
        bool ExistsOrderId(int id) const
        {
            bool ret = this->market_book.ExistsMarketOrderById(id);
        
            if (!ret)
            {
                ret = this->order_book.ExistsOrderId(id);
            }
        
            return ret;
        }
        
        void AddOrder(Order order)
        {
            if (order.order_type == ORDER_TYPE_MARKET)
            {
                this->market_book.AddMarketOrder(order);
            }
            else if (order.order_type == ORDER_TYPE_LIMIT)
            {
                this->order_book.AddOrder(order);
            }
            else
            {
                // Error
            }
        }
        
        void CancelOrder(Order order)
        {
            if (order.order_type == ORDER_TYPE_MARKET)
            {
                this->market_book.CancelMarketOrder(order);
            }
            else if (order.order_type == ORDER_TYPE_LIMIT)
            {
                this->order_book.CancelOrder(order);
            }
            else
            {
                // Error
            }
        }
        
        void CancelOrderById(int id)
        {
            if (this->market_book.ExistsMarketOrderById(id))
            {
                this->market_book.CancelMarketOrderById(id);
            }
            else
            {
                this->order_book.CancelOrderById(id);
            }
        }
        
        void PrintOrderBook()
        {
            this->order_book.PrintOrderBook();
        }
        
        void MatchOrderBook()
        {
            Order bid_order = this->order_book.GetBidFirst();
            Order ask_order = this->order_book.GetAskFirst();

            if ((bid_order.id != 0) && (ask_order.id != 0))
            {
                if (stof(bid_order.price) >= stof(ask_order.price))
                {
                    cout << "BOOK Order FILL: " << endl;
                    bid_order.PrintOrder();
                    ask_order.PrintOrder();

                    json j_data;
                    j_data["price"] = ask_order.price;
                    j_data["bid_order_id"] = bid_order.id;
                    j_data["ask_order_id"] = ask_order.id;

                    Event event(EVENT_ID_ORDER_FILL, j_data);
                    this->event_bus->Send(event);

                    this->order_book.CancelOrderById(bid_order.id);
                    this->order_book.CancelOrderById(ask_order.id);
                }
            }
        }
        
        void MatchMarketOrder()
        {
            Order market_order = this->market_book.GetFirst();
            Order book_order;
            json j_data;
        
            if(market_order.id == 0)
            {
                return;
            }

            if (market_order.order_side == ORDER_SIDE_BUY)
            {
                book_order = this->order_book.GetAskFirst();
                if(book_order.id == 0)
                {
                    return;
                }

                j_data["ask_order_id"] = book_order.id;
                j_data["bid_order_id"] = market_order.id;
            }
            else if (market_order.order_side == ORDER_SIDE_SELL)
            {
                book_order = this->order_book.GetBidFirst();
                if(book_order.id == 0)
                {
                    return;
                }

                j_data["ask_order_id"] = market_order.id;
                j_data["bid_order_id"] = book_order.id;
            }
            else
            {
                this->market_book.PopFirst();
        
                return;
            }
            j_data["price"] = book_order.price;
        
            cout << "Market Order FILL: " << endl;
            market_order.PrintOrder();
            book_order.PrintOrder();

            Event event(EVENT_ID_ORDER_FILL, j_data);
            this->event_bus->Send(event);
        
            this->order_book.CancelOrderById(book_order.id);
            this->market_book.PopFirst();
        }
        
        void Cyclic()
        {
            this->MatchOrderBook();
            this->MatchMarketOrder();
        }
        
        void run()
        {
            while (true)
            {
                this->Cyclic();
            }
        }
};

#endif
from ExchangeAPIWrapper.ExchangeAPIWrapper_Constants import*
from TimerUpdater.TimerUpdater import TimerUpdater
import dearpygui.dearpygui as dpg

class ExchangeUI:
    api_client = None

    def __init__(self, api_client):
        self.api_client = api_client
    
    def _UpdateOrderBook(self):
        body_json = self.api_client.GetOrderBook()
        if(body_json):
            order_book = body_json["data"]

            if( ("ask" in order_book.keys()) and isinstance(order_book["ask"], dict) ):
                temp = list(order_book["ask"].keys())
                temp.sort()
                temp = temp[: 5]

                prices = [float(price) for price in temp]
                amounts = [order_book["ask"][price] for price in temp]

                dpg.configure_item("STAIR_SERIES_ASK", x=prices, y=amounts)
            
            if( ("bid" in order_book.keys()) and isinstance(order_book["bid"], dict) ):
                temp = list(order_book["bid"].keys())
                temp.sort(reverse=True)
                temp = temp[: 5]
                
                prices = [float(price) for price in temp]
                amounts = [order_book["bid"][price] for price in temp]

                dpg.configure_item("STAIR_SERIES_BID", x=prices, y=amounts)
    
    def _GetCandles_DpgFriendly(self):
        data = None

        body_json = self.api_client.GetCandles(limit=100)
        if(body_json and body_json["data"]):
            candles = body_json["data"]

            timestamps = []
            opens = []
            closes = []
            highs = []
            lows = []

            for candle in candles:
                timestamps.append(candle["timestamp"])
                opens.append(float(candle["open"]))
                closes.append(float(candle["close"]))
                highs.append(float(candle["high"]))
                lows.append(float(candle["low"]))
            
            data = {}
            data["timestamps"] = timestamps
            data["opens"] = opens
            data["closes"] = closes
            data["highs"] = highs
            data["lows"] = lows
        
        return data
    
    def _UpdateCandles(self):
        data = self._GetCandles_DpgFriendly()      

        if(data != None):
            dpg.configure_item("CANDLE_SERIES", dates=data["timestamps"],
                opens=data["opens"], closes=data["closes"], lows=data["lows"], highs=data["highs"])
    
    def _GetTrades(self):
        data = None

        body_json = self.api_client.GetTrades(limit=20)
        if(body_json):
            data = body_json["data"]
        
        return data

    def _UpdateTrades(self):
        data = self._GetTrades()      

        if(data != None):
            dpg.delete_item("TARDES_TABLE", children_only=True, slot=1)

            for trade in data:
                with dpg.table_row(parent="TARDES_TABLE"):
                    dpg.add_text(trade["timestamp"])
                    dpg.add_text(trade["price"])
                    dpg.add_text(trade["quantity"])
    
    def _UpdaterFunc(self):
        self._UpdateCandles()
        self._UpdateOrderBook()
        self._UpdateTrades()
        
    
    def _ShowCandleChart(self, parent):
        timestamps = []
        opens = []
        closes = []
        highs = []
        lows = []

        data = self._GetCandles_DpgFriendly()

        if(data !=None):
            timestamps = data["timestamps"]
            opens = data["opens"]
            closes = data["closes"]
            highs = data["highs"]
            lows = data["lows"]

        with dpg.plot(parent=parent, label="Candle Series", height=-1, width=-1):
            dpg.add_plot_legend()
            xaxis = dpg.add_plot_axis(dpg.mvXAxis, label="Time", scale=dpg.mvPlotScale_Time)
            with dpg.plot_axis(dpg.mvYAxis, tag="CANDLES_Y_AXIS", label="USD", auto_fit=True):
                dpg.add_candle_series(timestamps, opens, closes, lows, highs, tag="CANDLE_SERIES", label="SYMBOL")
                dpg.fit_axis_data(dpg.top_container_stack())
            dpg.fit_axis_data(xaxis)

    def _ShowBookDepth(self, parent):        
        with dpg.plot(parent=parent, label="Order Book Depth", height=-1, width=-1):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="price", auto_fit=True)
            with dpg.plot_axis(dpg.mvYAxis, label="Amount", auto_fit=True):
                dpg.add_stair_series([], [], tag="STAIR_SERIES_ASK", label="ask", shaded=True)
                dpg.add_stair_series([], [], tag="STAIR_SERIES_BID", label="bid", shaded=True)
    
    def _ShowTrades(self, parent):
        with dpg.table(tag="TARDES_TABLE", label="Trades", header_row=True, borders_innerV=True):
            dpg.add_table_column(label="Time")
            dpg.add_table_column(label="Price")
            dpg.add_table_column(label="Quantity")
    
    def _AddOrder_Callback(self, sender, app_data, user_data):
        print(sender, app_data, user_data)

        price = dpg.get_value("ADD_ORDER_PRICE")
        price = "{:.2f}".format(price)
        amount = dpg.get_value("ADD_ORDER_AMOUNT")

        order_type = dpg.get_value("ADD_ORDER_TYPE")
        if("MARKET" == order_type):
            order_type = ORDER_TYPE_MARKET
        elif("LIMIT" == order_type):
            order_type = ORDER_TYPE_LIMIT
        else:
            raise Exception("Wrong Order Type: " + str(order_type))
        
        order_side = sender
        if("ADD_ORDER_BUY" == order_side):
            order_side = ORDER_SIDE_BUY
        elif("ADD_ORDER_SELL" == order_side):
            order_side = ORDER_SIDE_SELL
        else:
            raise Exception("Wrong Order Side: " + str(order_side))
        
        self.api_client.AddOrder(price=price, qty=amount, order_side=order_side, order_type=order_type)

    
    def _ShowBuySell(self, parent):
        dpg.add_combo(tag="ADD_ORDER_TYPE", parent=parent, label="Order Type", items=["MARKET", "LIMIT"], default_value="LIMIT")

        dpg.add_input_float(tag="ADD_ORDER_PRICE", parent=parent, label="Price", default_value=1.5, step=0.1)
        dpg.add_input_float(tag="ADD_ORDER_AMOUNT", parent=parent, label="Amount", default_value=100.0, step=1.0)

        with dpg.group(parent=parent, horizontal=True):
            dpg.add_button(tag="ADD_ORDER_BUY", label="BUY", callback=self._AddOrder_Callback)
            dpg.add_button(tag="ADD_ORDER_SELL", label="SELL", callback=self._AddOrder_Callback)


    def _ShowMainWindow(self, ):
        with dpg.child_window(parent="PRIMARY_WINDOW", resizable_x=False, resizable_y=True, height=600, width=-1) as child_w_0:
            with dpg.group(parent=child_w_0, horizontal=True) as group:
                with dpg.child_window(parent=group, resizable_x=True, resizable_y=False, width=700, border=False) as child_w_1:
                    self._ShowCandleChart(parent=child_w_1)
                with dpg.child_window(parent=group, resizable_x=True, resizable_y=False, width=700, border=False) as child_w_2:
                    self._ShowBookDepth(parent=child_w_2)
                with dpg.child_window(parent=group, resizable_x=False, resizable_y=False, width=-1, border=False) as child_w_3:
                    self._ShowTrades(parent=child_w_3)
        self._ShowBuySell(parent="PRIMARY_WINDOW")
    
    def Run(self):
        timer_updater = TimerUpdater(func=self._UpdaterFunc)

        dpg.create_context()
        dpg.create_viewport(title='Custom Title', width=900, height=700)

        dpg.add_window(tag="PRIMARY_WINDOW")
        self._ShowMainWindow()

        dpg.setup_dearpygui()
        dpg.show_viewport()
        dpg.set_primary_window("PRIMARY_WINDOW", True)
        
        timer_updater.Run()
        dpg.start_dearpygui()

        dpg.destroy_context()
import dearpygui.dearpygui as dpg
import threading
import time
import requests

def GetOrderBook():
    #print("GET ORDER BOOK")
    url = "https://127.0.0.1:8080/get_order_book"
    data = {}

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key)

    return (resp.status_code, resp.json())

def ShowBookDepth(parent):        
        with dpg.plot(parent=parent, label="Order Book Depth", height=400, width=-1):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="price", auto_fit=True)
            with dpg.plot_axis(dpg.mvYAxis, label="Amount", auto_fit=True):
                dpg.add_stair_series([1.0, 1.1], [2, 1], tag="stair_series_ask", label="ask", shaded=True)
                dpg.add_stair_series([1.3, 1.4], [1, 2], tag="stair_series_bid", label="bid", shaded=True)

def ShowMainWindow():
    dpg.add_input_int(default_value=0, tag="VAL", parent="PRIMARY_WINDOW")

    ShowBookDepth(parent="PRIMARY_WINDOW")
    

def TimerUpdate():
    i = 0

    while(True):
        time.sleep(1)

        i += 1
        dpg.set_value("VAL", i)

        #Get Order Book Data
        (status, body_json) = GetOrderBook()
        if(body_json):
            order_book = body_json

            if( ("ask" in order_book.keys()) and isinstance(order_book["ask"], dict) ):
                prices = [float(price) for price in list(order_book["ask"].keys())]
                amounts = list(order_book["ask"].values())

                dpg.configure_item("stair_series_ask", x=prices, y=amounts)
            
            if( ("bid" in order_book.keys()) and isinstance(order_book["bid"], dict) ):
                prices = [float(price) for price in list(order_book["bid"].keys())]
                amounts = list(order_book["bid"].values())

                dpg.configure_item("stair_series_bid", x=prices, y=amounts)

def main():
    t1 = threading.Thread(target=TimerUpdate)

    dpg.create_context()
    dpg.create_viewport(title='Custom Title', width=900, height=700)

    dpg.add_window(tag="PRIMARY_WINDOW")
    ShowMainWindow()

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.set_primary_window("PRIMARY_WINDOW", True)
    
    t1.start()
    dpg.start_dearpygui()

    dpg.destroy_context()

if(__name__ == "__main__"):
    main()
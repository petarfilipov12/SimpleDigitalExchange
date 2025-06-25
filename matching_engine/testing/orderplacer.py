import requests
import time
import json
import random
import threading

ORDER_SIDE_BUY = 0
ORDER_SIDE_SELL = 1
ORDER_SIDE_INVALID = 2

ORDER_TYPE_MARKET = 0
ORDER_TYPE_LIMIT = 1
ORDER_TYPE_INVALID = 2

def AddOrder(symbolP, priceP, qtyP, sideP, typeP):

    side_s = ""
    if(sideP == ORDER_SIDE_BUY):
        side_s = "ORDER_SIDE_BUY"
    elif(sideP == ORDER_SIDE_SELL):
        side_s = "ORDER_SIDE_SELL"
    else:
        side_s = "ORDER_SIDE_INVALID"

    type_s = ""
    if(typeP == ORDER_TYPE_MARKET):
        type_s = "ORDER_TYPE_MARKET"
    elif(typeP == ORDER_TYPE_LIMIT):
        type_s = "ORDER_TYPE_LIMIT"
    else:
        type_s = "ORDER_TYPE_INVALID"

    print("ADD ORDER", symbolP, priceP, qtyP, side_s, type_s)
    url = "https://127.0.0.1:8080/add_order"
    data = {}
    data["symbol"] = symbolP
    data["price"] = priceP
    data["quantity"] = qtyP
    data["order_side"] = sideP
    data["order_type"] = typeP

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def CancelOrder(symbol, order_idP):
    print("CANCEL ORDER")
    url = "https://127.0.0.1:8080/cancel_order"
    data = {}

    data["symbol"] = symbol
    data["order_id"] = order_idP

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def GetOrderBook(symbol):
    print("GET ORDER BOOK")
    url = "https://127.0.0.1:8080/get_order_book"
    data = {}

    data["symbol"] = symbol

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def RandomOrder(price_lim_min, price_lim_max):
    data = {}

    data["price"] = str(round(random.uniform(price_lim_min, price_lim_max), 2))
    data["qty"] = round(random.uniform(0, 500), 2)
    data["side"] = random.choice([ORDER_SIDE_BUY, ORDER_SIDE_SELL])
    #order_type = random.choices([ORDER_TYPE_MARKET, ORDER_TYPE_LIMIT], weights = [30, 70])[0]
    data["order_type"] = ORDER_TYPE_LIMIT

    return data

def InputOrder():
    data = {}

    data["symbol"] = input("symbol:")
    data["price"] = input("price:")
    data["qty"] = float(input("qty:"))
    data["side"] = int(input("side:"))
    data["order_type"] = int(input("order_type:"))

    return data

def AddOrderLoop(symbol, price_lim_min, price_lim_max):
    while(True):
        data = RandomOrder(price_lim_min, price_lim_max)
        data["symbol"] = symbol
        
        order = AddOrder(data["symbol"], data["price"], data["qty"], data["side"], data["order_type"])
        print(order)
        print()

        time.sleep(0.2)

def main():
    symbols_data = {}
    symbols_data["SYMBOL_1"] = {
        "price_lim_min": 1,
        "price_lim_max": 2
    }
    symbols_data["SYMBOL_2"] = {
        "price_lim_min": 10,
        "price_lim_max": 20
    }

    threads = []

    for symbol, symbol_data in symbols_data.items():
        args = [symbol]
        args += symbol_data.values()
        t = threading.Thread(target=AddOrderLoop, args=args)
        t.daemon = True
        t.start()

        threads.append(t)
    
    for t in threads:
        t.join()
    
    

    

if(__name__ == "__main__"):
    main()

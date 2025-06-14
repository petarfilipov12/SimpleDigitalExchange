import requests
import time
import json
import random

ORDER_SIDE_BUY = 0
ORDER_SIDE_SELL = 1
ORDER_SIDE_INVALID = 2

ORDER_TYPE_MARKET = 0
ORDER_TYPE_LIMIT = 1
ORDER_TYPE_INVALID = 2

def AddOrder(priceP, qtyP, sideP, typeP):

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

    print("ADD ORDER", priceP, qtyP, side_s, type_s)
    url = "https://127.0.0.1:8080/add_order"
    data = {}
    data["price"] = priceP
    data["quantity"] = qtyP
    data["order_side"] = sideP
    data["order_type"] = typeP

    path_to_pub_key = "../../../server_certs/cert2.pem"
    print("TUK1")
    resp = requests.post(url, json=data, verify=path_to_pub_key)#.json()
    print("TUK2")

    return resp

def CancelOrder(order_idP):
    print("CANCEL ORDER")
    url = "https://127.0.0.1:8080/cancel_order"
    data = {}
    data["order_id"] = order_idP

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def GetOrderBook():
    print("GET ORDER BOOK")
    url = "https://127.0.0.1:8080/get_order_book"
    data = {}

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def RandomOrder():
    data = {}

    data["price"] = str(round(random.uniform(1, 2), 2))
    data["qty"] = round(random.uniform(0, 500), 2)
    data["side"] = random.choice([ORDER_SIDE_BUY, ORDER_SIDE_SELL])
    #order_type = random.choices([ORDER_TYPE_MARKET, ORDER_TYPE_LIMIT], weights = [30, 70])[0]
    data["order_type"] = ORDER_TYPE_LIMIT

    return data

def InputOrder():
    data = {}

    data["price"] = input("price:")
    data["qty"] = float(input("qty:"))
    data["side"] = int(input("side:"))
    data["order_type"] = int(input("order_type:"))

    return data

def main():
    
    while(True):
        data = RandomOrder()
        #data = InputOrder()
        
        order = AddOrder(data["price"], data["qty"], data["side"], data["order_type"])
        print(order.content)
        print()

        #time.sleep(0.2)
        input("Press Enter")

    

if(__name__ == "__main__"):
    main()

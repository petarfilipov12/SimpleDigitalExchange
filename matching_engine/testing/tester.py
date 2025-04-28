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
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def GetOrder(order_idP):
    print("GET ORDER")
    url = "https://127.0.0.1:8080/get_order"
    data = {}
    data["order_id"] = order_idP

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

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

def main():
    
    # AddOrder("1.0", 100.0, ORDER_SIDE_BUY, ORDER_TYPE_LIMIT)
    # time.sleep(0.1)

    # AddOrder("1.1", 100.0,  ORDER_SIDE_BUY, ORDER_TYPE_LIMIT)
    # time.sleep(0.1)

    # AddOrder("1.3", 100.0,  ORDER_SIDE_SELL, ORDER_TYPE_LIMIT)
    # time.sleep(0.1)

    order = AddOrder("1.2", 100.0,  ORDER_SIDE_SELL, ORDER_TYPE_LIMIT)
    time.sleep(0.1)

    # AddOrder("0.0", 390.0,  ORDER_SIDE_SELL, ORDER_TYPE_MARKET)
    # time.sleep(0.1)

    print(order)
    print(GetOrder(order["data"]["order_id"]))

    

if(__name__ == "__main__"):
    main()
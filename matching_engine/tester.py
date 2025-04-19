import requests
import time
import json

ORDER_SIDE_BUY = 0
ORDER_SIDE_SELL = 1
ORDER_SIDE_INVALID = 2

ORDER_TYPE_MARKET = 0
ORDER_TYPE_LIMIT = 1
ORDER_TYPE_INVALID = 2

def AddOrder(priceP, sideP, typeP):
    print("ADD ORDER")
    url = "https://127.0.0.1:8080/add_order"
    data = {}
    data["price"] = priceP
    data["order_side"] = sideP
    data["order_type"] = typeP

    path_to_pub_key = "../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def CancelOrder(order_idP):
    print("CANCEL ORDER")
    url = "https://127.0.0.1:8080/cancel_order"
    data = {}
    data["order_id"] = order_idP

    path_to_pub_key = "../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def GetOrderBook():
    print("GET ORDER BOOK")
    url = "https://127.0.0.1:8080/get_order_book"
    data = {}

    path_to_pub_key = "../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def main():
    
    AddOrder("1.1", ORDER_SIDE_BUY, ORDER_TYPE_LIMIT)
    time.sleep(0.1)

    AddOrder("1.0", ORDER_SIDE_BUY, ORDER_TYPE_LIMIT)
    time.sleep(0.1)

    AddOrder("1.5", ORDER_SIDE_SELL, ORDER_TYPE_LIMIT)
    time.sleep(0.1)

    AddOrder("1.2", ORDER_SIDE_SELL, ORDER_TYPE_LIMIT)
    time.sleep(0.1)


    print(GetOrderBook())

    

if(__name__ == "__main__"):
    main()
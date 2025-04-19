import matplotlib.pyplot as plt
import requests
import time
import json

def GetOrderBook():
    #print("GET ORDER BOOK")
    url = "https://127.0.0.1:8080/get_order_book"
    data = {}

    path_to_pub_key = "../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key).json()

    return resp

def main():
    plt.ion()

    order_book = GetOrderBook()["data"]

    graph = plt.barh(list(order_book["bid"].keys()), list(order_book["bid"].values()))[0]
    plt.pause(1)

    while(True):
        order_book = GetOrderBook()["data"]

        if( ("bid" in order_book.keys()) and ("ask" in order_book.keys()) and (order_book["bid"]) and (order_book["ask"])):
            graph.remove()
            graph = plt.barh(list(order_book["bid"].keys()), list(order_book["bid"].values()))[0]
        else:
            print(order_book)

        plt.pause(0.5)


    

    

if(__name__ == "__main__"):
    main()
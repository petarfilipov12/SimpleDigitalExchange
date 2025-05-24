
import requests
import time
import json
import os
from termcolor import colored
import datetime

class Logger:
    file_path = "error.log"
    def __init__(self, file_path = "error.log"):
        self.file_path = file_path
    
    def log(self, s):
        with open(self.file_path, "a") as f:
            f.write(s + '\n')

def GetOrderBook():
    #print("GET ORDER BOOK")
    url = "https://127.0.0.1:8080/get_order_book"
    data = {}

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key)

    return (resp.status_code, resp.json())

def main():

    logger = Logger()

    while(True):
        (status, body_json) = GetOrderBook()
        if(body_json):
            os.system("clear")
            order_book = body_json

            if( ("ask" in order_book.keys()) and isinstance(order_book["ask"], dict) ):
                for price, amount in reversed(order_book["ask"].items()):
                    print(colored(price + ":" + str(amount), "red"))

            print("------------------------------------")

            if( ("bid" in order_book.keys()) and isinstance(order_book["bid"], dict) ):
                for price, amount in reversed(order_book["bid"].items()):
                    print(colored(price + ":" + str(amount), "green"))

        else:
            print(status, "ERROR")
            logger.log(str(datetime.datetime.now()) + ": " + str(status) + ":" + str(body_json))
        
        time.sleep(0.3)

if(__name__ == "__main__"):
    main()

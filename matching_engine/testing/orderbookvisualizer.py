import pandas as pd 
import datetime as dt 

# plotting packages 
import plotly.graph_objects as go
import plotly.express as px
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import seaborn as sns

# hide warnings 
import warnings
warnings.filterwarnings('ignore')

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
    
    fig, ax = plt.subplots()
    ax.set_xlabel("Price")
    ax.set_ylabel("Amount")

    def update(frame):
        (status, body_json) = GetOrderBook()
        if(body_json):
            order_book = body_json

            ax.clear()
            ax.cla()
            os.system("clear")

            if( ("ask" in order_book.keys()) and isinstance(order_book["ask"], dict) ):
                # book_ask_df = pd.DataFrame(body_json["ask"].items(), columns=["price", "amount"])
                # book_ask_df["price"] = book_ask_df["price"].astype(float)

                # sns.ecdfplot(x="price", weights="amount", stat="count", data=book_ask_df, ax=ax, color='r')

                ax.bar([float(price) for price in list(order_book["ask"].keys())], list(order_book["ask"].values()), color='red')

                #print
                for price, amount in reversed(order_book["ask"].items()):
                    print(colored(price + ":" + str(amount), "red"))

            print("------------------------------------")

            if( ("bid" in order_book.keys()) and isinstance(order_book["bid"], dict) ):
                # book_bid_df = pd.DataFrame(body_json["bid"].items(), columns=["price", "amount"])
                # book_bid_df["price"] = book_bid_df["price"].astype(float)

                # sns.ecdfplot(x="price", weights="amount", stat="count", complementary=True, data=book_bid_df, ax=ax, color='g')
            
                ax.bar([float(price) for price in list(order_book["bid"].keys())], list(order_book["bid"].values()), color='green')


                #print
                for price, amount in reversed(order_book["bid"].items()):
                    print(colored(price + ":" + str(amount), "green"))

            fig.canvas.draw()
        else:
            print(status, "ERROR")
            logger.log(str(datetime.datetime.now()) + ": " + str(status) + ":" + str(body_json))

    anim = FuncAnimation(fig, update)
    plt.show()


    

    

if(__name__ == "__main__"):
    main()

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

import random
import requests


def GetOrderBook():
    #print("GET ORDER BOOK")
    url = "https://127.0.0.1:8080/get_order_book"
    data = {}

    path_to_pub_key = "../../../server_certs/cert2.pem"
    resp = requests.post(url, json=data, verify=path_to_pub_key)

    return (resp.status_code, resp.json())


    

def main():
    #book_json = {'ask': {'1.99': 76.20999908447266}, 'bid': {'1.09': 334.6000061035156, '1.13': 275.67999267578125, '1.18': 185.1499786376953, '1.81': 164.70005798339844}}

    fig, ax = plt.subplots()
    ax.set_xlabel("Price")
    ax.set_ylabel("Amount")

    def update(frame):
        (status, body_json) = GetOrderBook()
        if(body_json):
            book_ask_df = pd.DataFrame(body_json["ask"].items(), columns=["price", "amount"])
            book_ask_df["price"] = book_ask_df["price"].astype(float)

            book_bid_df = pd.DataFrame(body_json["bid"].items(), columns=["price", "amount"])
            book_bid_df["price"] = book_bid_df["price"].astype(float)

            ax.clear()

            sns.ecdfplot(x="price", weights="amount", stat="count", complementary=True, data=book_bid_df, ax=ax, color='g')
            sns.ecdfplot(x="price", weights="amount", stat="count", data=book_ask_df, ax=ax, color='r')

            fig.canvas.draw()

    anim = FuncAnimation(fig, update)
    plt.show()

if(__name__ == "__main__"):
    main()

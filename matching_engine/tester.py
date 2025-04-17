import requests
import time
import json

def main():
    url = "https://127.0.0.1:8080/add_order"
    data = {}
    data["price"] = "1.0"
    data["order_side"] = 0
    data["order_type"] = 1

    path_to_pub_key = "../../server_certs/cert2.pem"

    resp = requests.post(url, json=data, verify=path_to_pub_key)

    print(resp.json())

if(__name__ == "__main__"):
    main()
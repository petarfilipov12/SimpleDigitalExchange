from .ExchangeAPIWrapper_Constants import*
import requests

class ExchangeAPIWrapper:
    api_endpoint = None
    path_to_pub_key = None

    def __init__(self, api_endpoint, path_to_pub_key):
        self.path_to_pub_key = path_to_pub_key.strip()

        self.api_endpoint = api_endpoint.strip()
        if(self.api_endpoint[-1] == '/'):
            self.api_endpoint = self.api_endpoint[: -1]
    
    def _CheckStatusCode(self, status_code):
        if( (status_code < 200) or (status_code > 299) ):
            raise Exception("Connection error, Status code: " + str(status_code))

    def _Request_POST(self, service_path, json_data=None):
        service_path = service_path.strip()
        if(service_path[0] != '/'):
            service_path = '/' + service_path

        resp = requests.post(self.api_endpoint + service_path, json=json_data, verify=self.path_to_pub_key)

        self._CheckStatusCode(resp.status_code)

        return resp.json()
    
    def AddOrder(self, price, qty, order_side, order_type):
        if( (order_side < 0) or (order_side >= ORDER_SIDE_INVALID)):
            raise Exception("Invalid Order Side: " + str(order_side))
        
        if( (order_type < 0) or (order_type >= ORDER_TYPE_INVALID)):
            raise Exception("Invalid Order Type: " + str(order_type))

        data = {}
        data["price"] = price
        data["quantity"] = qty
        data["order_side"] = order_side
        data["order_type"] = order_type

        return self._Request_POST("/add_order", json_data=data)
    
    def CancelOrder(self, order_id):
        data = {}
        data["order_id"] = order_id

        return self._Request_POST("/cancel_order", json_data=data)
    
    def GetOrder(self, order_id):
        data = {}
        data["order_id"] = order_id

        return self._Request_POST("/get_order", json_data=data)
    
    def GetOrderBook(self):
        return self._Request_POST("/get_order_book")
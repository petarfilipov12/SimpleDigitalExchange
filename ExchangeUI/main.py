from ExchangeAPIWrapper.ExchangeAPIWrapper import ExchangeAPIWrapper
from ExchangeUI import ExchangeUI

def main():
    api_client = ExchangeAPIWrapper("https://127.0.0.1:8080", "../../server_certs/cert2.pem")
    exchange_ui = ExchangeUI(api_client)
    exchange_ui.Run()



if(__name__ == "__main__"):
    main()
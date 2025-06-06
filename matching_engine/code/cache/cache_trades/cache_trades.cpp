#include "cache_trades.h"

CacheTrades::CacheTrades() {}
CacheTrades::~CacheTrades() {}

ReturnType CacheTrades::OrderFilled(string price, float amount)
{
    this->trades_lock.lock();
    this->trades.push_back((struct sTrade){price, amount});
    this->trades_lock.unlock();
    
    return RET_OK;
}

ReturnType CacheTrades::GetTrades(int limit, json *data)const
{
    vector<sTrade> temp;
    vector<sTrade>::size_type trades_size;

    this->trades_lock.lock();
    trades_size = this->trades.size();
    this->trades_lock.unlock();

    if(trades_size > 0)
    {
        if (limit > trades_size)
        {
            limit = trades_size;
        }

        this->trades_lock.lock();
        temp = vector<sTrade>((this->trades.end() - limit), this->trades.end());
        this->trades_lock.unlock();

        reverse(temp.begin(), temp.end());

        *data = temp;
    }

    return RET_OK;
}
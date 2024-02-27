#pragma once
#include "../DataStructure/Queue.h"
#include "../Event/TradeEvent.h"
#include "../Util/Util.h"
#include "Book.h"
#include "Order.h"
#include <map>
#include <utility>
#include <list>

namespace OrderBook {
    using orderID = int;

    class OrderBook {
    public:
        OrderBook() = default;
        EventStatus submitOrder(orderID ID, int quantity, int price, TradeDirection tradeDirection);
        EventStatus updateOrder(orderID ID, int quantity, int price, TradeDirection tradeDirection);
        EventStatus deleteOrder(orderID ID, TradeDirection tradeDirection);
        EventStatus executeOrder(orderID ID, int quantity, TradeDirection tradeDirection);

    private:
        void updateBestBidAndOffer();
        Book<TradeDirection::Buy> buyBook;
        Book<TradeDirection::Sell> sellBook;
        BestPriceLevel bestBid;
        BestPriceLevel bestOffer;
    };
}// namespace OrderBook
#pragma once
#include "../Util/Util.h"
#include "../DataStructure/Queue.h"
#include "../Event/TradeEvent.h"
#include "Book.h"
#include "Order.h"
#include <list>
#include <map>
#include <utility>

namespace OrderBook {
    using orderID = int;

    class orderBookTest {
    public:
        orderBookTest() = default;
        EventStatus submitOrder(orderID ID, unsigned int quantity, unsigned int price, TradeDirection tradeDirection);
        EventStatus updateOrder(orderID ID, unsigned int quantity, unsigned int price, TradeDirection tradeDirection);
        EventStatus deleteOrder(orderID ID, TradeDirection tradeDirection);
        EventStatus executeOrder(orderID ID, unsigned int quantity, TradeDirection tradeDirection);

    private:
        void updateBestBidAndOffer();
        Book<TradeDirection::Buy> buyBook;
        Book<TradeDirection::Sell> sellBook;
        BestPriceLevel bestBid;
        BestPriceLevel bestOffer;
    };
}// namespace OrderBook
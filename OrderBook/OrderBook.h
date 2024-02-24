#pragma once
#include "../DataStructure/Queue.h"
#include "../Event/TradeEvent.h"
#include "../Util/Util.h"
#include "Book.h"
#include "Order.h"
#include <map>
#include <utility>
namespace OrderBook {
    class OrderBook {
    public:
        using orderID = unsigned int;
        using eQueue = QueueForMultiThread<TradeEvent>;
        OrderBook() = default;
        OrderBook(eQueue queue): eventQueue(std::move(queue)) {};
        void ListenEvent() {}
    private:
        Book<TradeDirection::Buy> buyBook;
        Book<TradeDirection::Sell> sellBook;
        PriceLevel *bestBid{nullptr};
        PriceLevel *bestOffer{nullptr};
        std::map<orderID, Order *> orderMap{};
        eQueue eventQueue;
    };
}// namespace OrderBook
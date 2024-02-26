#pragma once

#include "../Event/TradeEvent.h"
#include "Order.h"
#include <list>
#include <vector>

namespace OrderBook {
    class BestPriceLevel {
        int quantity{}, price{};
    };

    class PriceLevel {
    public:
        PriceLevel() = default;
        explicit PriceLevel(int price): price(price){};
        std::tuple<EventStatus, std::list<Order>::iterator> submitOrder(orderID ID, int quantity, int price);
        EventStatus updateOrder(Order * order, int quantity, int price);
        EventStatus deleteOrder(Order * order);
        EventStatus executeOrder(Order * order);
    private:
        std::list<Order> orderList;
        int quantity{}, price{};
    };
}// namespace OrderBook
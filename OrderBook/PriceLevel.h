#pragma once

#include "../Event/TradeEvent.h"
#include "Order.h"
#include <list>
#include <vector>
#include <unordered_map>

namespace OrderBook {
    class BestPriceLevel {
    public:
        BestPriceLevel()= default;
        BestPriceLevel(int quantity, int price) : quantity(quantity), price(price) {}
        int quantity{}, price{};
    };

    class PriceLevel {
    public:
        PriceLevel() = default;
        explicit PriceLevel(int price) : price(price){};
        std::tuple<EventStatus, std::list<Order>::iterator> submitOrder(orderID ID, int quantity, int price);
        EventStatus updateOrder(std::list<Order>::iterator order, int quantity, int price);
        EventStatus deleteOrder(std::list<Order>::iterator order);
        EventStatus executeOrder(std::list<Order>::iterator order);
        int getQuantity() const { return quantity; }
        int getPrice() const { return price; }
        size_t getOrderLength() const { return orderList.size(); }

    private:
        std::list<Order> orderList;
        int quantity{}, price{};
    };
}// namespace OrderBook
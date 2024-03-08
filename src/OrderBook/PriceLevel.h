#pragma once

#include "../Event/TradeEvent.h"
#include "Order.h"
#include <list>
#include <unordered_map>
#include <vector>

namespace OrderBook {
    class BestPriceLevel {
    public:
        BestPriceLevel() = default;
        BestPriceLevel(int quantity, int price) : quantity(quantity), price(price) {}
        int quantity{}, price{};
    };

    class PriceLevel {
    public:
        PriceLevel() = default;
        explicit PriceLevel(unsigned int price) : price(price) {
            assert(price > 0);
        };

        std::tuple<EventStatus, std::list<Order>::iterator> submitOrder(orderID ID, unsigned int newQuantity, unsigned int newPrice);
        EventStatus updateOrder(std::list<Order>::iterator order, unsigned int newQuantity, unsigned int newPrice);
        EventStatus cancelOrder(std::list<Order>::iterator order);
        EventStatus executeOrder(std::list<Order>::iterator order, unsigned int executeQuantity);
        unsigned getQuantity() const { return quantity; }
        unsigned getPrice() const { return price; }
        unsigned getOrderLength() const { return orderList.size(); }

    private:
        std::list<Order> orderList;
        unsigned int quantity{0}, price{0};
    };
}// namespace OrderBook
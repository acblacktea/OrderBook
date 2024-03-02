#pragma once

namespace OrderBook {
    class Order {
    public:
        Order() = default;
        Order(int orderID, int quantity, int price)
            :orderID(orderID),quantity(quantity),price(price){}
        unsigned int orderID{};
        unsigned int quantity{};
        unsigned int price{};
    };
}// namespace OrderBook
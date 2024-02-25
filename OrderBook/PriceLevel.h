#pragma once

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

    private:
        std::list<Order> orderList;
        int quantity{}, price{};
    };
}// namespace OrderBook
#pragma once

#include "Order.h"
#include <list>
#include <vector>

namespace OrderBook {
    class PriceLevel {
    public:
        PriceLevel() = default;
        void insertOrder(Order &order) {
            orderList.emplace_back(&order);
        }

        void updateOrder(Order &order, int newPrice, int newQuantity) {
            auto orderIter = std::find(orderList.begin(), orderList.end(), &order);
            if (orderIter == orderList.end()) {
                // TODO emit error log;
                return;
            }

            (*orderIter)->price = newPrice;
            (*orderIter)->quantity = newQuantity;

        }
        void removeOrder(Order &order) {
            auto orderIter = std::find(orderList.begin(), orderList.end(), &order);
            if (orderIter == orderList.end()) {
                // TODO emit error log;
                return;
            }

            delete (*orderIter);
            orderList.erase(orderIter);
        }

    private:
        int limitPrice{0};
        int totalVolume{0};
        std::list<Order*> orderList;
    };
}// namespace OrderBook
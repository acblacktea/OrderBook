#pragma once

#include "../Util/Util.h"
#include "OrderBook.h"
#include "PriceLevel.h"
#include <map>
#include <unordered_map>

namespace OrderBook {
    template<TradeDirection buyOrSell>
    class Book {
    public:
        EventStatus submitOrder(orderID ID, int quantity, int price);
        EventStatus updateOrder(orderID ID, int quantity, int price);
        EventStatus deleteOrder(orderID ID);
        EventStatus executeOrder(orderID ID, int quantity);
        BestPriceLevel GetBestPriceLevel();
    private:
        TradeDirection side{buyOrSell};
        int totalQuantity;
        std::unordered_map<price, PriceLevel> priceLevelMap{};
        std::unordered_map<price, PriceLevel*> priceLevelOrderMap{};
        std::unordered_map<orderID, std::list<Order>::iterator> ID2OrderMap{};
        std::unordered_map<orderID, PriceLevel*> ID2PriceLevelMap{};
    };

    /// submit O(1)
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::submitOrder(orderID ID, int quantity, int price) {
        if (ID2OrderMap.find(ID) != ID2OrderMap.end()) {
            return submitRepeatedOrder;
        }

        PriceLevel *priceLevel = nullptr;

        // priceLevelInit
        if (priceLevelMap.find(price) == priceLevelMap.end()) {
            priceLevelMap[price] = PriceLevel(0);
            priceLevel = &priceLevelMap[price];

            // o(logN)
            priceLevelOrderMap[price] = priceLevel;
        } else {
            priceLevel = &priceLevelMap[price];
        }

        auto [status, orderIterator] = priceLevel->submitOrder(ID, quantity, price);
        if (status != success) {
            // TODO add rollback logic.
            return status;
        }

        ID2PriceLevelMap[ID] = priceLevel;
        ID2OrderMap[ID] = orderIterator;
        totalQuantity += quantity;

        return status;
    }

    // GetBestPriceLevel O(1)
    template<TradeDirection buyOrSell>
    BestPriceLevel Book<buyOrSell>::GetBestPriceLevel() {
        return {};
    }

    // executeOrder O(1)
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::executeOrder(orderID ID, int quantity) {
        return fail;
    }

    // deleteOrder O(1)
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::deleteOrder(orderID ID) {
        return fail;
    }

    // updateOrder O(1)
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::updateOrder(orderID ID, int quantity, int price) {
        return fail;
    }
}// namespace OrderBook
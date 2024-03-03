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
        EventStatus submitOrder(orderID ID, unsigned int quantity, unsigned int price);
        EventStatus updateOrder(orderID ID, unsigned int quantity, unsigned int price);
        EventStatus deleteOrder(orderID ID);
        EventStatus executeOrder(orderID ID, unsigned int quantity);
        BestPriceLevel GetBestPriceLevel();
    private:
        TradeDirection side{buyOrSell};
        int totalQuantity{};
        std::unordered_map<price, PriceLevel> priceLevelMap{};
        std::map<price, PriceLevel*> priceLevelOrderMap{};
        std::unordered_map<orderID, std::list<Order>::iterator> ID2OrderMap{};
        std::unordered_map<orderID, PriceLevel*> ID2PriceLevelMap{};
    };

    /// submit O(1)
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::submitOrder(orderID ID, unsigned int quantity, unsigned int price) {
        if (ID2OrderMap.find(ID) != ID2OrderMap.end()) {
            return EventStatus::SubmitRepeatedOrder;
        }

        PriceLevel *priceLevel = nullptr;

        // priceLevelInit
        if (priceLevelMap.find(price) == priceLevelMap.end()) {
            priceLevelMap[price] = PriceLevel(price);
            priceLevel = &priceLevelMap[price];

            // o(logN)
            priceLevelOrderMap[price] = priceLevel;
        } else {
            priceLevel = &priceLevelMap[price];
        }

        auto [status, orderIterator] = priceLevel->submitOrder(ID, quantity, price);
        if (status != EventStatus::Success) {
            // TODO How to handle rollback?
            return status;
        }

        ID2PriceLevelMap[ID] = priceLevel;
        ID2OrderMap[ID] = orderIterator;
        totalQuantity += quantity;

        return status;
    }

    /// GetBestPriceLevel O(1)
    template<TradeDirection buyOrSell>
    BestPriceLevel Book<buyOrSell>::GetBestPriceLevel() {
        BestPriceLevel bestPriceLevel;
        if (!priceLevelOrderMap.empty()) {
            std::map<price, PriceLevel*>::iterator bestPrice;
            if (side == TradeDirection::Buy) {
                bestPrice = priceLevelOrderMap.begin();
            } else if (side == TradeDirection::Sell && !priceLevelOrderMap.empty()){
                bestPrice = --priceLevelOrderMap.end();
            }

            bestPriceLevel = BestPriceLevel(bestPrice->second->getQuantity(), bestPrice->second->getPrice());
        }

        return bestPriceLevel;
    }

    // executeOrder O(1)
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::executeOrder(orderID ID, unsigned int quantity) {
        if (ID2OrderMap.find(ID) == ID2OrderMap.end()) {
            return EventStatus::ExecuteNonExistedOrder;
        }

        auto &iterator = ID2OrderMap[ID];
        auto priceLevel = priceLevelOrderMap[ID];
        auto status = priceLevel->executeOrder(iterator);
        if (status != EventStatus::Success) {
            return status;
        }

        if (!priceLevel->getOrderLength()) {
            auto price = priceLevel->getPrice();
            priceLevelMap.erase(price);
            priceLevelOrderMap.erase(price);
        }

        ID2OrderMap.erase(ID);
        ID2PriceLevelMap.erase(ID);
        totalQuantity -= quantity;
        return status;
    }

    /// deleteOrder O(1)
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::deleteOrder(orderID ID) {
        if (ID2OrderMap.find(ID) == ID2OrderMap.end()) {
            return EventStatus::DeleteNonExistedOrder;
        }

        auto &iterator = ID2OrderMap[ID];
        auto priceLevel = priceLevelOrderMap[ID];
        auto quantity = priceLevel->getQuantity();
        auto status = priceLevel->cancelOrder(iterator);
        if (status != EventStatus::Success) {
            return status;
        }

        if (!priceLevel->getOrderLength()) {
            auto price = priceLevel->getPrice();
            priceLevelMap.erase(price);
            priceLevelOrderMap.erase(price);
        }

        ID2OrderMap.erase(ID);
        ID2PriceLevelMap.erase(ID);
        totalQuantity -= quantity;
        return status;
    }

    /// updateOrder O(1) reduce price or quantity;
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::updateOrder(orderID ID, unsigned int quantity, unsigned int price) {
        if (ID2OrderMap.find(ID) == ID2OrderMap.end()) {
            return EventStatus::UpdateNonExistedOrder;
        }

        auto &beforeIterator = ID2OrderMap[ID];
        auto beforePriceLevel = priceLevelOrderMap[ID];
        auto beforeQuantity = beforePriceLevel->getQuantity();
        auto beforePrice = beforePriceLevel->getPrice();
        auto status = EventStatus::Success;

        if (price == beforePrice) {
            status = quantity? beforePriceLevel->updateOrder(beforeIterator, quantity, price) : beforePriceLevel->cancelOrder(beforeIterator);
            if (status != EventStatus::Success) {
                return status;
            }

            if (!beforePriceLevel->getOrderLength()) {
                priceLevelMap.erase(price);
                priceLevelOrderMap.erase(price);
            }
        } else {
            status = beforePriceLevel->cancelOrder(beforeIterator);
            if (status != EventStatus::Success) {
                return status;
            }

            if (!beforePriceLevel->getOrderLength()) {
                priceLevelMap.erase(beforePrice);
                priceLevelOrderMap.erase(beforePrice);
            }

            ID2OrderMap.erase(ID);
            ID2PriceLevelMap.erase(ID);

            if (auto nowStatus = submitOrder(ID, quantity, price); nowStatus != EventStatus::Success) {
                /// todo add logic roll back
                return nowStatus;
            }
        }

        totalQuantity -= beforeQuantity - quantity;
        return status;
    }

}// namespace OrderBook
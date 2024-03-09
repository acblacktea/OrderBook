#pragma once

#include "../Util/Util.h"
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

        // only used for testing now, real production will use event driven way transmit information.
        const PriceLevel *GetPriceLevelByPrice(unsigned int price) const;
        const Order *GetOrderByOrderID(orderID ID) const;
        const PriceLevel *GetPriceLevelByOrderID(orderID ID) const;
        unsigned int GetTotalQuantity() { return totalQuantity; }

    private:
        TradeDirection side{buyOrSell};
        int totalQuantity{0};
        std::unordered_map<price, PriceLevel> priceLevelMap{};
        std::map<price, PriceLevel *> priceLevelOrderMap{};
        std::unordered_map<orderID, std::list<Order>::iterator> ID2OrderMap{};
        std::unordered_map<orderID, PriceLevel *> ID2PriceLevelMap{};
    };

    /// GetBestPriceLevel O(1)
    template<TradeDirection buyOrSell>
    BestPriceLevel Book<buyOrSell>::GetBestPriceLevel() {
        if (!priceLevelOrderMap.empty()) {
            std::map<price, PriceLevel *>::iterator bestPrice;
            if (side == TradeDirection::Buy) {
                bestPrice = priceLevelOrderMap.begin();
            } else if (side == TradeDirection::Sell) {
                bestPrice = --priceLevelOrderMap.end();
            }

            return BestPriceLevel(bestPrice->second->getQuantity(), bestPrice->second->getPrice());
        }

        return BestPriceLevel{-1,-1};
    }

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

    // executeOrder O(1)
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::executeOrder(orderID ID, unsigned int quantity) {
        if (ID2OrderMap.find(ID) == ID2OrderMap.end()) {
            return EventStatus::ExecuteNonExistedOrder;
        }

        auto &iterator = ID2OrderMap[ID];
        auto priceLevel = ID2PriceLevelMap[ID];
        auto price = priceLevel->getPrice();
        auto status = priceLevel->executeOrder(iterator, quantity);
        if (status != EventStatus::Success) {
            return status;
        }

        if (!iterator->quantity) {
            ID2OrderMap.erase(ID);
            ID2PriceLevelMap.erase(ID);
        }

        if (!priceLevel->getOrderLength()) {
            priceLevelMap.erase(price);
            priceLevelOrderMap.erase(price);
        }

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
        auto orderQuantity = iterator->quantity;
        auto priceLevel = ID2PriceLevelMap[ID];
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
        totalQuantity -= orderQuantity;
        return status;
    }

    /// updateOrder O(1) reduce price or quantity;
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::updateOrder(orderID ID, unsigned int quantity, unsigned int price) {
        if (ID2OrderMap.find(ID) == ID2OrderMap.end()) {
            return EventStatus::UpdateNonExistedOrder;
        }

        auto &beforeIterator = ID2OrderMap[ID];
        auto beforePriceLevel = ID2PriceLevelMap[ID];
        auto beforeQuantity = beforeIterator->quantity;
        auto beforePrice = beforeIterator->price;
        auto status = EventStatus::Success;

        if (price == beforePrice) {
            // only support reduce price
            if (quantity) {
                if (status = beforePriceLevel->updateOrder(beforeIterator, quantity, price); status != EventStatus::Success) {
                    return status;
                }
            } else {
                if (status = beforePriceLevel->cancelOrder(beforeIterator); status != EventStatus::Success) {
                    return status;
                }

                ID2OrderMap.erase(ID);
                ID2PriceLevelMap.erase(ID);
            }


            if (!beforePriceLevel->getOrderLength()) {
                priceLevelMap.erase(price);
                priceLevelOrderMap.erase(price);
            }

            totalQuantity -= beforeQuantity - quantity;
        } else {
            if (status = beforePriceLevel->cancelOrder(beforeIterator); status != EventStatus::Success) {
                return status;
            }

            ID2OrderMap.erase(ID);
            ID2PriceLevelMap.erase(ID);
            if (!beforePriceLevel->getOrderLength()) {
                priceLevelMap.erase(beforePrice);
                priceLevelOrderMap.erase(beforePrice);
            }

            if (quantity) {
                if (status = submitOrder(ID, quantity, price); status != EventStatus::Success) {
                    /// todo add logic roll back
                    return status;
                }
            }

            totalQuantity -= beforeQuantity;
        }

        return status;
    }

    // currently only used by test, not concurrent safe
    template<TradeDirection buyOrSell>
    const Order *Book<buyOrSell>::GetOrderByOrderID(orderID ID) const {
        auto iter = ID2OrderMap.find(ID);
        if (iter != ID2OrderMap.end()) {
            return &(*(iter->second));
        }

        return nullptr;
    }

    // currently only used by test, not concurrent safe
    template<TradeDirection buyOrSell>
    const PriceLevel *Book<buyOrSell>::GetPriceLevelByPrice(unsigned int price) const {
        auto iter = priceLevelMap.find(price);
        if (iter != priceLevelMap.end()) {
            return &(iter->second);
        }

        return nullptr;
    }

    template<TradeDirection buyOrSell>
    const PriceLevel *Book<buyOrSell>::GetPriceLevelByOrderID(orderID ID) const {
        auto iter = ID2PriceLevelMap.find(ID);
        if (iter != ID2PriceLevelMap.end()) {
            return &(*(iter->second));
        }

        return nullptr;
    }

}// namespace OrderBook
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
        EventStatus submitOrder(orderID ID, int quantity, int price) const;
        EventStatus updateOrder(orderID ID, int quantity, int price) const;
        EventStatus deleteOrder(orderID ID) const;
        EventStatus executeOrder(orderID ID, int quantity) const;
        BestPriceLevel GetBestPriceLevel() const;
    private:
        TradeDirection side{buyOrSell};
        int quantity;
        std::map<price, PriceLevel> priceLevelMap;
        std::unordered_map<orderID, std::list<Order>::iterator> ID2OrderMap{};
        std::unordered_map<orderID, std::list<std::map<price, PriceLevel>>> ID2PriceLevelMap{};
    };

    template<TradeDirection buyOrSell>
    BestPriceLevel Book<buyOrSell>::GetBestPriceLevel() const {
        return {};
    }

    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::executeOrder(orderID ID, int quantity) const {
        return fail;
    }
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::deleteOrder(orderID ID) const {
        return fail;
    }
    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::updateOrder(orderID ID, int quantity, int price) const {
        return fail;
    }

    template<TradeDirection buyOrSell>
    EventStatus Book<buyOrSell>::submitOrder(orderID ID, int quantity, int price) const {
        return fail;
    }
}// namespace OrderBook
#pragma once

#include "../Util/Util.h"
#include "OrderBook.h"
#include "PriceLevel.h"
#include <map>

namespace OrderBook {
    template<TradeDirection buyOrSell>
    class Book {
    public:
    private:
        using price = unsigned int;
        TradeDirection side{buyOrSell};
        std::map<price, PriceLevel> priceLevelMap;
    };
}// namespace OrderBook
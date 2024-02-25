#pragma once

namespace OrderBook {
    using orderID = int;
    using price = unsigned int;

    enum class TradeDirection {
        Sell = -1,
        Buy = 1
    };

    enum ErrorCoded {};
}
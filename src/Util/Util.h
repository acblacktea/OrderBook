#pragma once

namespace OrderBook {
    using orderID = unsigned int;
    using price = unsigned int;

    enum class TradeDirection {
        Sell = -1,
        Buy = 1
    };

    enum ErrorCoded {};

    constexpr auto inputQueueSize = 10000000;
    constexpr auto outputQueueSize = 10000000;
}
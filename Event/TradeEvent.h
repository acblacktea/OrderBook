#pragma once
#include "../Util/Util.h"
namespace OrderBook {
    enum class EventType {
        Default = 0,
        Cancellation,
        Deletion,
        VisibleLimitOrderExecution,
        HiddenLimitOrderExecution,
        CrossTrade, // unsupported
        TradingHaltIndicator
    };

    class TradeEvent {
    public:
        TradeEvent() = default;
        TradeEvent(unsigned long long timestamp,
                   EventType eventType,
                   int orderID,
                   unsigned int shareSize,
                   unsigned int price,
                   TradeDirection direction) {
            this->timestamp = timestamp;
            this->eventType = eventType;
            this->orderID = orderID;
            this->shareSize = shareSize;
            this->price = price;
            this->direction = direction;
        }

        unsigned long long timestamp{};
        EventType eventType{EventType::Default};
        int orderID{};
        unsigned int shareSize{};
        unsigned int price{};
        TradeDirection direction;
    };
}// namespace OrderBook
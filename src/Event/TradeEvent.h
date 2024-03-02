#pragma once
#include "../Util/Util.h"
namespace OrderBook {
    enum class EventType {
        Default = 0,
        Submit,
        Cancellation,
        Deletion,
        VisibleLimitOrderExecution,
        HiddenLimitOrderExecution, // unsupported
        CrossTrade, // unsupported
        TradingHaltIndicator
    };

    enum class EventStatus {
        Pending = 0,
        Success,
        Fail,
        Unsupported,
        Illegal,
        SubmitRepeatedOrder,
        ExecuteNonExistedOrder,
        DeleteNonExistedOrder,
        UpdateNonExistedOrder
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
            this->status = EventStatus::Pending;
        }

        unsigned long long timestamp{};
        EventType eventType{EventType::Default};
        int orderID{};
        unsigned int shareSize{};
        unsigned int price{};
        TradeDirection direction;
        EventStatus status;
    };


}// namespace OrderBook
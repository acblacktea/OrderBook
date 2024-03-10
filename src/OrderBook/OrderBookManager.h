#pragma once
#include "../Util/Util.h"
#include "OrderBook.h"
#include "unordered_map"
namespace OrderBook {
    enum class OrderState {
        failSubmit,
        insideBook,
        executed,
        cancel,
    };


    class OrderBookManager {
    public:
        using eQueue = SPSCLockFreeRingQueue<TradeEvent>;
        OrderBookManager() = default;
        OrderBookManager(eQueue *inputQueue, eQueue *outputQueue)
            :eventInputQueue{inputQueue}, eventOutputQueue{outputQueue} {};
        void ListenEvent();
        TradeEvent getHandledEvent() {
            return eventOutputQueue->pop();
        }

        using orderInfo = class {
            OrderState state;
            TradeDirection direction;
        };
    private:
        OrderBook orderBook; // currently only support one order book, in future will support multi order book
        eQueue *eventInputQueue{};
        eQueue *eventOutputQueue{};
        std::unordered_map<int, OrderState> ordersState;

        EventStatus submitOrder(const TradeEvent &event);
        EventStatus updateOrder(const TradeEvent &event);
        EventStatus deleteOrder(const TradeEvent &event);
        EventStatus executeVisibleOrder(const TradeEvent &event);
    };
}

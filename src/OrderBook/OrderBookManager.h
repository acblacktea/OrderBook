#pragma once
#include "../Util/Util.h"
#include "OrderBook.h"
#include "unordered_map"
namespace OrderBook {
    class OrderBookManager {
    public:
        using eQueue = LockFreeRingQueue<TradeEvent>;
        OrderBookManager() = default;
        OrderBookManager(const eQueue *inputQueue, const eQueue *outputQueue)
            :eventInputQueue{inputQueue}, eventOutputQueue{outputQueue} {};
        void ListenEvent();

    private:

        OrderBook orderBook; // currently only support one order book, in future will support multi order book
        const eQueue *eventInputQueue{};
        const eQueue *eventOutputQueue{};

        EventStatus submitOrder(const TradeEvent &event);
        EventStatus updateOrder(const TradeEvent &event);
        EventStatus deleteOrder(const TradeEvent &event);
        EventStatus executeVisibleOrder(const TradeEvent &event);
    };
}
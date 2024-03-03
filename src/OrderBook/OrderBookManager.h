#pragma once
#include "../Util/Util.h"
#include "OrderBook.h"
#include "unordered_map"
namespace OrderBook {
    class orderBookManagerTest {
    public:
        using eQueue = LockFreeRingQueue<TradeEvent>;
        orderBookManagerTest() = default;
        orderBookManagerTest(eQueue *inputQueue, eQueue *outputQueue)
            :eventInputQueue{inputQueue}, eventOutputQueue{outputQueue} {};
        void ListenEvent();

    private:
        orderBookTest orderBook; // currently only support one order book, in future will support multi order book
        eQueue *eventInputQueue{};
        eQueue *eventOutputQueue{};

        EventStatus submitOrder(const TradeEvent &event);
        EventStatus updateOrder(const TradeEvent &event);
        EventStatus deleteOrder(const TradeEvent &event);
        EventStatus executeVisibleOrder(const TradeEvent &event);
    };
}
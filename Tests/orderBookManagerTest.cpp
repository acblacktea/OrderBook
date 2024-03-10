#include "OrderBook/OrderBookManager.h"
#include "DataStructure/Queue.h"
#include "Event/TradeEvent.h"
#include "OrderListener/GenerateDataListener.h"
#include <gtest/gtest.h>
#include <thread>
using namespace OrderBook;
TEST(orderBookManager, test_tick2tick) {
    SPSCLockFreeRingQueue<TradeEvent> inputQueue(inputQueueSize);
    SPSCLockFreeRingQueue<TradeEvent> outputQueue(outputQueueSize);
    auto generateDataListener = GenerateDataListener("./testCases/testData", &inputQueue);
    auto orderBookManager = OrderBookManager(&inputQueue, &outputQueue);

    std::thread orderProducer{[&] { generateDataListener.listen(); }};
    std::thread orderConsumer{[&] { orderBookManager.ListenEvent(); }};
    std::thread output{[&] {
        int cnt = 0;
        while (true) {
            auto event = orderBookManager.getHandledEvent();
            ++cnt;
            EXPECT_EQ(event.status, EventStatus::Success);
            if (event.eventType == EventType::TradingHaltIndicator) {
                break;
            }
        }

        EXPECT_EQ(cnt, 8);
    }};
    orderProducer.join();
    orderConsumer.join();
    output.join();
}
#pragma once
#include "../DataStructure/Queue.h"
#include "../Event/TradeEvent.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <utility>
namespace OrderBook {
    class GenerateDataListener {
    public:
        using eQueue = LockFreeRingQueue<TradeEvent>;
        GenerateDataListener() = default;
        explicit GenerateDataListener(std::string filePath, eQueue *queue): filePath(std::move(filePath)), eventInputQueue(queue) {}
        void listen();

    private:
        const eQueue *eventInputQueue{};
        std::string filePath;
    };
}// namespace OrderBook
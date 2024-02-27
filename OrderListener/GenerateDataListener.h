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
        using eQueue = QueueForMultiThread<TradeEvent>;
        GenerateDataListener() = default;
        std::regex rgx = std::regex("(.+),(.+),(.+),(.+),(.+),(.+)");
        std::smatch matches;
        explicit GenerateDataListener(std::string filePath, eQueue *queue): filePath(std::move(filePath)), eventInputQueue(queue) {}
        void listen() {
            std::string line;
            std::ifstream file("filePath");

            while(std::getline(std::cin, line)) {
                TradeEvent tradeEvent;
                if (std::regex_search(line, matches, rgx)) {
                    tradeEvent.timestamp = std::stoi(matches[0].str());
                    tradeEvent.eventType = static_cast<EventType>(std::stoi(matches[1].str()));
                    tradeEvent.orderID = std::stoi(matches[2].str());
                    tradeEvent.shareSize = std::stoi(matches[3].str());
                    tradeEvent.price = std::stoi(matches[4].str());
                    tradeEvent.direction = static_cast<TradeDirection>(std::stoi(matches[5].str()));
                } else {
                    tradeEvent.status = illegal;
                }

                eventInputQueue->pushData(tradeEvent);
            }

            file.close();
        }

    private:
        const eQueue *eventInputQueue{};
        std::string filePath;
    };
}// namespace OrderBook
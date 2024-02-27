//
// Created by ByteDance on 28/2/24.
//
#include "GenerateDataListener.h"
namespace OrderBook {
    void OrderBook::GenerateDataListener::listen() {
        std::string line;
        std::ifstream file("filePath");
        std::regex rgx = std::regex("(.+),(.+),(.+),(.+),(.+),(.+)");
        std::smatch matches;

        while (std::getline(std::cin, line)) {
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
}// namespace OrderBook

#pragma once
#include "../Queue/Queue.h"
#include <string>
#include <utility>
namespace OrderBook {
    class GenerateDataListener {
    public:
        GenerateDataListener() = default;
        explicit GenerateDataListener(std::string filePath): filePath(std::move(filePath)) {}
        void listen() {
            for ()
        }
    private:
        std::string filePath;
    };
}// namespace OrderBook
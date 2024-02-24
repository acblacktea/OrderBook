#pragma once
#include <vector>

namespace OrderBook {
    template<typename T>
    class Queue {
    public:
        Queue() = default;
        Queue(int size) {
            data.reserve(size);
        }

    private:
        std::vector<T> data;
    };

    template<typename T>
    class QueueForMultiThread {
    public:
        QueueForMultiThread() = default;
        QueueForMultiThread(int size) {
            data.reserve(size);
        }

    private:
        std::vector<T> data;
    };
}

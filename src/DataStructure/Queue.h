#pragma once
#include <atomic>
#include <tuple>
#include <vector>
#include <iostream>

namespace OrderBook {
    // lock free queue, thread safety, one produce, one consumer
    template<typename T>
    class SPSCLockFreeRingQueue {
    public:
        SPSCLockFreeRingQueue() = default;
        explicit SPSCLockFreeRingQueue(int size) {
            data.reserve(size);
            for (auto i = 0; i < size; ++i) {
                data.emplace_back(T());
            }

            capicity = size;
        }

        // must success
        void push(T value) {
            auto tail = tailAtomic.load();
            auto nextTail = (tail + 1) % capicity;
            auto head = 0;
            do {
                head = headAtomic.load();
            } while(nextTail == head);
            data[tail] = value;
            tailAtomic.store(nextTail, std::memory_order::release);
        }

        T pop() {
            int tail;
            int head = headAtomic.load();
            do {
                tail = tailAtomic.load();
            } while(head == tail);

            int nextHead = (head + 1) % capicity;
            headAtomic.store(nextHead, std::memory_order::release);
            return data[head];
        }

        size_t size() {
            auto tail = tailAtomic.load();
            auto head = headAtomic.load();
            return ((tail - head) % capicity + capicity) % capicity;
        }

    private:
        std::atomic<int> tailAtomic{0}, headAtomic{0};
        int capicity{0};
        std::vector<T> data;
    };

    template<typename T>
    class RingQueue {
    public:
        RingQueue() = default;
        explicit RingQueue(int size) {
            data.reserve(size);
            for (auto i = 0; i < size; ++i) {
                data.emplace_back(T());
            }

            capicity = size;
        }

        // must success
        void push(T value) {
            auto nextTail = (tail + 1) % capicity;
            do {} while(nextTail == head);

            std::lock_guard<std::mutex> lock(mutex);
            data[tail] = value;
            tail = nextTail;
        }

        T pop() {
            do {} while(head == tail);

            std::lock_guard<std::mutex> lock(mutex);
            auto value = data[head];
            head = (head + 1) % capicity;
            return value;
        }

        size_t size() {
            return ((tail - head) % capicity + capicity) % capicity;
        }

    private:
        int tail{0}, head{0};
        int capicity{0};
        std::mutex mutex;
        std::vector<T> data;
    };


    /// simple implementation of lock free queue, dont scold me....
    /// has concurrent bug now.
    template<typename T>
    class MPMCLockFreeRingQueue {
    public:
        using Meta = class QueueInfo {
        public:
            int head, tail, len, placeHolder /*guarantee lock free*/;
        };

        MPMCLockFreeRingQueue() {
            assert(std::atomic_is_lock_free(&atomicStatus));
        };

        explicit MPMCLockFreeRingQueue(int size) {
            assert(std::atomic_is_lock_free(&atomicStatus));
            data.reserve(size);
            for (auto i = 0; i < size; ++i) {
                data.emplace_back(T());
            }

            capicity = size;
            atomicStatus.store({0, -1, 0, 0}, std::memory_order_relaxed);
        }

        void push(T value) {
            while(!tryPush(value));
        }

        T pop() {
            T value;
            while(!tryPop(value)) {}
            return value;
        }


        inline bool tryPush(T& value) {
            Meta status = atomicStatus.load(std::memory_order_acq_rel);
            int nextTail = (status.tail + 1) % capicity;;
            if (status.len < capicity && atomicStatus.compare_exchange_weak(status, {status.head, nextTail, status.len + 1, 0}, std::memory_order_acq_rel)) {
                data[nextTail] = value;
                return true;
            }

            return false;
        }


        inline bool tryPop(T &value) {
            Meta status = atomicStatus.load(std::memory_order_acq_rel);
            int nextHead = (status.head + 1) % capicity;
            if (status.len > 0 && atomicStatus.compare_exchange_weak(status, {nextHead, status.tail, status.len - 1, 0}, std::memory_order_acq_rel)) {
                value = T(data[status.head]);
                return true;
            }

            return false;
        }

        size_t size() {
            auto status = atomicStatus.load(std::memory_order_acq_rel);
            return status.len;
        }


    private:
        std::vector<T> data;
        std::atomic<Meta> atomicStatus{{0, -1, 0, 0}};
        int capicity{0};
    };
}// namespace OrderBook

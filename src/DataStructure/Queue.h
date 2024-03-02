#pragma once
#include <atomic>
#include <tuple>
#include <vector>
namespace OrderBook {

    /// lock free queue just simple implementation, dont scold me....
    /// i need to do more test to gurantee that this implementation doesnt have any problems
    template<typename T>
    class LockFreeRingQueue {
    public:
        using Meta = class QueueInfo {
        public:
            int head, tail;
        };

        LockFreeRingQueue() {
            assert(std::atomic_is_lock_free(&atomicStatus));
        };
        explicit LockFreeRingQueue(int size) {
            assert(std::atomic_is_lock_free(&atomicStatus));
            data.reserve(size);
            capicity = size;
            atomicStatus.store({0, -1}, std::memory_order_relaxed);
        }

        // must success
        void push(T value) {
            int nextTail;
            Meta status;
            do {
                status = atomicStatus.load(std::memory_order_relaxed);
                nextTail = (status.tail + 1) % capicity;
            } while (status.tail - status.head + 1 < capicity && atomicStatus.compare_exchange_weak(status, {status.head, nextTail}));

            data[nextTail] = value;
        }

        // must success
        T pop() {
            int nextHead;
            Meta status;
            do {
                status = atomicStatus.load(std::memory_order_relaxed);
                nextHead = (status.head + 1) % capicity;
            } while (status.tail - status.head + 1 >= 0 && atomicStatus.compare_exchange_weak(status, {nextHead, status.tail}));

            return data[nextHead];
        }


    private:
        std::vector<T> data;
        std::atomic<Meta> atomicStatus{{0, -1}};
        int capicity{0};
    };


    template<typename T>
    class RingQueue {
    public:
        RingQueue() = default;
        explicit RingQueue(int size) {
            data.reserve(size);
            capicity = size;
        }

        // must success
        void push(T value) {
            while (tail - head + 1 >= capicity) {}
            int nextTail = (tail + 1) % capicity;;
            data[nextTail] = value;
            tail = nextTail;
        }

        T pop() {
            while (tail - head + 1 == 0) {}
            auto currentHead = head;
            head = (head + 1) % capicity;;
            return data[currentHead];
        }

    private:
        int tail{-1}, head{0};
        int capicity{0};
        std::vector<T> data;
    };
}// namespace OrderBook

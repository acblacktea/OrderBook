#pragma once
#include <atomic>
#include <tuple>
#include <vector>
namespace OrderBook {

    /// simple implementation of lock free queue, dont scold me....
    /// i need to do more test to gurantee that this implementation doesnt have any concurrent problems
    template<typename T>
    class LockFreeRingQueue {
    public:
        using Meta = class QueueInfo {
        public:
            int head, tail, len, placeHolder /*guarantee lock free*/;
        };

        LockFreeRingQueue() {
            assert(std::atomic_is_lock_free(&atomicStatus));
        };

        explicit LockFreeRingQueue(int size) {
            assert(std::atomic_is_lock_free(&atomicStatus));
            data.reserve(size);
            for (auto i = 0; i < size; ++i) {
                data.emplace_back(T());
            }

            capicity = size;
            atomicStatus.store({0, -1, 0, 0}, std::memory_order_relaxed);
        }

        // must success
        void push(T value) {
            int nextTail;
            Meta status;
            do {
                status = atomicStatus.load(std::memory_order_relaxed);
                nextTail = (status.tail + 1) % capicity;
            } while (status.len >= capicity || !atomicStatus.compare_exchange_weak(status, {status.head, nextTail, status.len + 1, 0}));

            data[nextTail] = value;
        }

        // must success
        T pop() {
            int nextHead;
            int head;
            Meta status;
            do {
                status = atomicStatus.load(std::memory_order_relaxed);
                nextHead = (status.head + 1) % capicity;

            } while (status.len <= 0 || !atomicStatus.compare_exchange_weak(status, {nextHead, status.tail, status.len - 1, 0}));

            return data[status.head];
        }

        size_t size() {
            auto status = atomicStatus.load(std::memory_order_relaxed);
            return status.len;
        }


    private:
        std::vector<T> data;
        std::atomic<Meta> atomicStatus{{0, -1, 0, 0}};
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
            while (len >= capicity) {}
            int nextTail = (tail + 1) % capicity;;
            data[nextTail] = value;
            tail = nextTail;
            ++len;
        }

        T pop() {
            while (len == 0) {}
            auto currentHead = head;
            head = (head + 1) % capicity;
            --len;
            return data[currentHead];
        }

    private:
        int tail{-1}, head{0}, len;
        int capicity{0};
        std::vector<T> data;
    };
}// namespace OrderBook

#include <DataStructure/Queue.h>
#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <unordered_set>

TEST(lock_free_queue, test_basic_function_basic) {
    auto count = 1000000;
    OrderBook::LockFreeRingQueue<int> queue(count + 1000);
    auto counter = std::unordered_set<int>();
    for (auto i = 0; i < count; ++i) {
        queue.push(i);
    }

    for (auto i = 0; i < count; ++i) {
        auto value = queue.pop();
        counter.insert(value);
    }

    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(counter.size(), count);

    for (auto i = 0; i < count; ++i) {
        EXPECT_TRUE(counter.find(i) != counter.end());
    }
}


TEST(lock_free_queue, test_basic_function) {
    auto count = 100000;
    OrderBook::LockFreeRingQueue<int> queue(2);
    auto counter = std::unordered_set<int>();
    std::thread produce{[&] {
        for (auto i = 0; i < count; ++i) {
            queue.push(i);
        }
    }};

    std::thread consumer{[&] {
        for (auto i = 0; i < count; ++i) {
            auto value = queue.pop();
            counter.insert(value);
        }

        EXPECT_EQ(queue.size(), 0);
        EXPECT_EQ(counter.size(), count);
        for (auto i = 0; i < count; ++i) {
            EXPECT_TRUE(counter.find(i) != counter.end());
        }
    }};

    produce.join();
    consumer.join();
}

TEST(lock_free_queue, test_multi_thread_basic_function) {
    auto count = 1000000;
    auto thread = 10;
    OrderBook::LockFreeRingQueue<int> queue(1000);
    auto counter = std::unordered_set<int>();
    auto threads = std::vector<std::thread>();

    std::mutex mutex;

    for (auto i = 0; i < thread; ++i) {
        auto index = i;
        threads.emplace_back([&] {
            auto left = index * count, right = (index + 1) * count;
            for (auto j = left ; j < right; ++j) {
                queue.push(j);
            }
        });
    }


    for (auto i = 0; i < thread; ++i) {
        auto index = i;
        threads.emplace_back([&] {
            auto left = index * count, right = (index + 1) * count;
            for (auto j = left ; j < right; ++j) {
                auto value = queue.pop();
                const std::lock_guard<std::mutex> lock(mutex);
                counter.insert(value);
            }
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }

    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(counter.size(), count * thread);
    for (auto i = 0; i < count; ++i) {
        //EXPECT_TRUE(counter.find(i) != counter.end());
    }
}
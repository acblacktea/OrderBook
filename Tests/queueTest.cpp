#include <DataStructure/Queue.h>
#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <unordered_set>
#include <chrono>

class QueueHelperTest : public testing::Test {
public:
    static void test_benchmark_consumer_speed_less_than_producer(int count, int queueSize) {
        std::cout << "total input data size: " << count << " queue size " << queueSize << std::endl;
        test_lockQueueBenchmark_consumer_speed_less_than_producer(count, queueSize);
        test_lockFreeQueueBenchmark_consumer_speed_less_than_producer(count, queueSize);
        std::cout << std::endl;

    }

    static void test_lockQueueBenchmark_consumer_speed_less_than_producer(int count, int queueSize) {
        OrderBook::RingQueue<int> queue(queueSize);
        auto counter = std::unordered_set<int>();
        auto start = std::chrono::high_resolution_clock::now();
        std::thread produce{[&] {
            for (auto i = 0; i < count; ++i) {
                queue.push(i);
            }
        }};

        std::thread consumer{[&] {
            for (auto i = 0; i < count; ++i) {
                auto value = queue.pop();
                counter.insert(value);
                counter.insert(value);
            }
        }};

        produce.join();
        consumer.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "lock queue Time taken: " << duration.count() << " nanoseconds" << std::endl;
    }

    static void test_lockFreeQueueBenchmark_consumer_speed_less_than_producer(int count, int queueSize) {
        OrderBook::SPSCLockFreeRingQueue<int> queue(queueSize);
        auto counter = std::unordered_set<int>();

        auto start = std::chrono::high_resolution_clock::now();
        ;
        std::thread produce{[&] {
            for (auto i = 0; i < count; ++i) {
                queue.push(i);
            }
        }};

        std::thread consumer{[&] {
            for (auto i = 0; i < count; ++i) {
                auto value = queue.pop();
                counter.insert(value);
                counter.insert(value);
            }
        }};

        produce.join();
        consumer.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "lock free queue Time taken: " << duration.count() << " nanoseconds" << std::endl;
    }


    static void test_benchmark_producer_speed_less_than_consumer(int count, int queueSize) {
        std::cout << "total input data size: " << count << " queue size " << queueSize << std::endl;
        test_lockQueueBenchmark_producer_speed_less_than_consumer(count, queueSize);
        test_lockFreeQueueBenchmark_producer_speed_less_than_consumer(count, queueSize);
        std::cout << std::endl;

    }

    static void test_lockQueueBenchmark_producer_speed_less_than_consumer(int count, int queueSize) {
        OrderBook::RingQueue<int> queue(queueSize);
        auto counter = std::unordered_set<int>();
        auto start = std::chrono::high_resolution_clock::now();
        std::thread produce{[&] {
            for (auto i = 0; i < count; ++i) {
                counter.insert(i);
                queue.push(i);
            }
        }};

        std::thread consumer{[&] {
            for (auto i = 0; i < count; ++i) {
                queue.pop();
            }
        }};

        produce.join();
        consumer.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "lock queue Time taken: " << duration.count() << " nanoseconds" << std::endl;
    }

    static void test_lockFreeQueueBenchmark_producer_speed_less_than_consumer(int count, int queueSize) {
        OrderBook::SPSCLockFreeRingQueue<int> queue(queueSize);
        auto counter = std::unordered_set<int>();

        auto start = std::chrono::high_resolution_clock::now();
        ;
        std::thread produce{[&] {
            for (auto i = 0; i < count; ++i) {
                counter.insert(i);
                queue.push(i);
            }
        }};

        std::thread consumer{[&] {
            for (auto i = 0; i < count; ++i) {
                queue.pop();
            }
        }};

        produce.join();
        consumer.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "lock free queue Time taken: " << duration.count() << " nanoseconds" << std::endl;
    }


    static void test_benchmark_producer_consumer_speed_equal(int count, int queueSize) {
        std::cout << "total input data size: " << count << " queue size " << queueSize << std::endl;
        test_lockQueueBenchmark_producer_consumer_speed_equal(count, queueSize);
        test_lockFreeQueueBenchmark_producer_consumer_speed_equal(count, queueSize);
        std::cout << std::endl;

    }

    static void test_lockQueueBenchmark_producer_consumer_speed_equal(int count, int queueSize) {
        OrderBook::RingQueue<int> queue(queueSize);
        auto start = std::chrono::high_resolution_clock::now();
        std::thread produce{[&] {
            for (auto i = 0; i < count; ++i) {
                queue.push(i);
            }
        }};

        std::thread consumer{[&] {
            for (auto i = 0; i < count; ++i) {
                queue.pop();
            }
        }};

        produce.join();
        consumer.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "lock queue Time taken: " << duration.count() << " nanoseconds" << std::endl;
    }

    static void test_lockFreeQueueBenchmark_producer_consumer_speed_equal(int count, int queueSize) {
        OrderBook::SPSCLockFreeRingQueue<int> queue(queueSize);
        auto start = std::chrono::high_resolution_clock::now();
        ;
        std::thread produce{[&] {
            for (auto i = 0; i < count; ++i) {
                queue.push(i);
            }
        }};

        std::thread consumer{[&] {
            for (auto i = 0; i < count; ++i) {
                queue.pop();
            }
        }};

        produce.join();
        consumer.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "lock free queue Time taken: " << duration.count() << " nanoseconds" << std::endl;
    }
};

TEST(lock_free_queue, test_basic_function_basic) {
    auto count = 1000000;
    OrderBook::MPMCLockFreeRingQueue<int> queue(count + 1000);
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
    OrderBook::MPMCLockFreeRingQueue<int> queue(100000);
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
    OrderBook::MPMCLockFreeRingQueue<int> queue(1000);
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

TEST(lock_free_ring_queue, test_basic_function_basic) {
    auto count = 1000000;
    OrderBook::SPSCLockFreeRingQueue<int> queue(count + 1000);
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

TEST(ring_queue, test_basic_function_basic) {
    auto count = 1000000;
    OrderBook::RingQueue<int> queue(count + 1000);
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

TEST(lock_free_ring_queue, test_single_producer_single_consumer_function) {
    constexpr auto count = 100000000;
    OrderBook::SPSCLockFreeRingQueue<int> queue(5);
    auto counter = std::unordered_set<int>();
    ;
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

TEST(ring_queue, test_single_producer_single_consumer_function) {
    constexpr auto count = 100000000;
    OrderBook::RingQueue<int> queue(5);
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


TEST(QueueHelperTest, test_benchmark) {
    std::cout << "producer speed nearly equal to consumer speed " << std::endl;
    QueueHelperTest::test_benchmark_producer_consumer_speed_equal(100000000, 10);
    QueueHelperTest::test_benchmark_producer_consumer_speed_equal(100000000, 100);
    QueueHelperTest::test_benchmark_producer_consumer_speed_equal(100000000, 1000);
    QueueHelperTest::test_benchmark_producer_consumer_speed_equal(100000000, 10000);
    QueueHelperTest::test_benchmark_producer_consumer_speed_equal(100000000, 100000);
    QueueHelperTest::test_benchmark_producer_consumer_speed_equal(100000000, 1000000);


    std::cout << "consumer speed < produce speed " << std::endl;
    QueueHelperTest::test_benchmark_consumer_speed_less_than_producer(100000000, 10);
    QueueHelperTest::test_benchmark_consumer_speed_less_than_producer(100000000, 100);
    QueueHelperTest::test_benchmark_consumer_speed_less_than_producer(100000000, 1000);
    QueueHelperTest::test_benchmark_consumer_speed_less_than_producer(100000000, 10000);
    QueueHelperTest::test_benchmark_consumer_speed_less_than_producer(100000000, 100000);
    QueueHelperTest::test_benchmark_consumer_speed_less_than_producer(100000000, 1000000);


    std::cout << "producer speed < consumer speed " << std::endl;
    QueueHelperTest::test_benchmark_producer_speed_less_than_consumer(100000000, 10);
    QueueHelperTest::test_benchmark_producer_speed_less_than_consumer(100000000, 100);
    QueueHelperTest::test_benchmark_producer_speed_less_than_consumer(100000000, 1000);
    QueueHelperTest::test_benchmark_producer_speed_less_than_consumer(100000000, 10000);
    QueueHelperTest::test_benchmark_producer_speed_less_than_consumer(100000000, 100000);
    QueueHelperTest::test_benchmark_producer_speed_less_than_consumer(100000000, 1000000);
}
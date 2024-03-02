cmake_minimum_required(VERSION 3.26)
project(OrderBook)

set(CMAKE_CXX_STANDARD 23)

add_executable(OrderBook src/main.cpp
        src/Event/TradeEvent.h
        src/MemoryPool/MemoryPool.h
        src/OrderBook/OrderBook.h
        src/OrderBook/OrderBook.h
        src/OrderBook/Book.h
        src/OrderListener/NasdaqListener.h
        src/OrderListener/GenerateDataListener.h
        src/OrderListener/LobsterDataListener.h
        src/Util/DebugDisplayTool.h
        src/OrderBook/PriceLevel.h
        src/OrderBook/Order.h
        src/Util/Util.h
        src/Util/ErrorCode.h
        src/Util/ErrorTracker.h
        src/Util/Log.h
        src/Util/PerformanceTool.h
        src/DataStructure/Queue.h
        src/OrderBook/PriceLevel.cpp
        src/OrderBook/OrderBook.cpp
        src/OrderBook/OrderBookManager.h
        src/OrderBook/OrderBookManager.cpp
        src/OrderListener/LondonExchangeListener.h
        src/OrderListener/GenerateDataListener.cpp
        test.cpp
        src/main.cpp)

cmake_minimum_required(VERSION 3.26)
project(OrderBook2)

set(CMAKE_CXX_STANDARD 23)

add_executable(OrderBook2 main.cpp
        Event/TradeEvent.h
        MemoryPool/MemoryPool.h
        OrderBook/OrderBook.h
        OrderBook/OrderBook.h
        OrderBook/Book.h
        OrderListener/FIXListener.h
        OrderListener/GenerateDataListener.h
        OrderListener/LobsterDataListener.h
        Util/DebugDisplayTool.h
        OrderBook/PriceLevel.h
        OrderBook/Order.h
        Util/Util.h
        Util/ErrorCode.h
        Util/ErrorTracker.h
        Util/Log.h
        Util/PerformanceTool.h
        DataStructure/Queue.h
        OrderBook/PriceLevel.cpp)

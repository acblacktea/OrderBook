#pragma once
#include <unordered_set>
#include <vector>

/** a simple implementation for fixed size memory pool,
  * only can be used in single thread, not thread safety.
  */

namespace OrderBook {
    const auto defaultSize = 1000000;

    template<typename T, size_t dataSize = defaultSize>
    class MemoryPool {
    public:
        MemoryPool() = default;
        explicit MemoryPool(int size) {
            dataSet.reserve(size);
        }

        bool TryAllocateData(T *data) const {
            // TODO
            data = &dataSet[0];
        }

        T *AllocateData() const {
            // TODO
        }

        void FreeData(T *data) {
        }

    private:
        std::vector<T> dataSet;
        std::unordered_set<int> usedDataIndexMap;
        std::unordered_set<int> freeDataIndexMap;
    };
}
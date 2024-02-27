#include "./OrderBook/OrderBookManager.h"
#include "./OrderListener/GenerateDataListener.h"
#include <string>
#include <thread>
using namespace OrderBook;
int main(int argc, char *argv[]) {
    std::string filePath;

    for (auto i = 0; i < argc; ++i) {
        std::string parameter = std::string(argv[i]);
        if (parameter == "-f") {
            if (i + 1 < argc) {
                filePath = parameter;
                ++i;
            }
        }
    }

    QueueForMultiThread<TradeEvent> inputQueue;
    QueueForMultiThread<TradeEvent> outputQueue;
    auto generateDataListener = GenerateDataListener(filePath, &inputQueue);
    auto orderBookManager = OrderBookManager(&inputQueue, &outputQueue);

    std::thread orderProducer{[&] { generateDataListener.listen(); }};
    std::thread orderConsumer{[&] { orderBookManager.ListenEvent(); }};

    orderProducer.join();
    orderConsumer.join();
}

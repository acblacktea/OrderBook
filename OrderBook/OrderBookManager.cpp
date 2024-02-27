#include "OrderBookManager.h"
namespace OrderBook {
    void OrderBookManager::ListenEvent() {
        while (eventInputQueue != nullptr) {
            auto event = eventInputQueue->popData();
            if (event.status != pending) {
                eventOutputQueue->pushData(event);
                continue;
            }

            switch (event.eventType) {
                case EventType::Submit:
                    event.status = submitOrder(event);
                    break;
                case EventType::Cancellation:
                    event.status = updateOrder(event);
                    break;
                case EventType::Deletion:
                    event.status = deleteOrder(event);
                    break;
                case EventType::VisibleLimitOrderExecution:
                    event.status = executeVisibleOrder(event);
                    break;
                case EventType::HiddenLimitOrderExecution:// unsupported
                    event.status = unsupported;
                    break;
                case EventType::CrossTrade:// unsupported
                    event.status = unsupported;
                    break;
                case EventType::TradingHaltIndicator:
                    event.status = success;
                    break;
                default:
                    event.status = unsupported;
                    ;
            }

            eventOutputQueue->pushData(event);
        }
    }

    EventStatus OrderBookManager::submitOrder(const TradeEvent &event) {
        return orderBook.submitOrder(event.orderID, event.shareSize, event.price, event.direction);
    }
    EventStatus OrderBookManager::updateOrder(const TradeEvent &event) {
        return orderBook.updateOrder(event.orderID, event.shareSize, event.price, event.direction);
    }
    EventStatus OrderBookManager::deleteOrder(const TradeEvent &event) {
        return orderBook.deleteOrder(event.orderID, event.direction);
    }
    EventStatus OrderBookManager::executeVisibleOrder(const TradeEvent &event) {
        return orderBook.executeOrder(event.orderID, event.shareSize, event.direction);
    }

}// namespace OrderBook
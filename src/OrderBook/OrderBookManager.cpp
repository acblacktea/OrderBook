#include "OrderBookManager.h"
namespace OrderBook {
    void OrderBookManager::ListenEvent() {
        while (eventInputQueue != nullptr) {
            auto event = eventInputQueue->pop();
            if (event.status != EventStatus::Pending) {
                eventOutputQueue->push(event);
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
                    event.status = EventStatus::Unsupported;
                    break;
                case EventType::CrossTrade:// unsupported
                    event.status = EventStatus::Unsupported;
                    break;
                case EventType::TradingHaltIndicator:
                    event.status = EventStatus::Success;
                    break;
                default:
                    event.status = EventStatus::Unsupported;
                    ;
            }

            eventOutputQueue->push(event);
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
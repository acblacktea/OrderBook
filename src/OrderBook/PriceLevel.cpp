#include "PriceLevel.h"
namespace OrderBook {

    std::tuple<EventStatus, std::list<Order>::iterator> PriceLevel::submitOrder(orderID ID, int quantity, int price) {
        orderList.emplace_back(ID, quantity, price);
        return {EventStatus::Success, --orderList.end()};
    }

    EventStatus PriceLevel::updateOrder(std::list<Order>::iterator order, int quantity, int price) {
        order->quantity = quantity;
        order->price = price;
        return EventStatus::Success;
    }

    EventStatus PriceLevel::deleteOrder(std::list<Order>::iterator order) {
        orderList.erase(order);
        return EventStatus::Success;
    }

    EventStatus PriceLevel::executeOrder(std::list<Order>::iterator order) {
        orderList.erase(order);
        // TODO add log
        return EventStatus::Success;
    }
}
#include "PriceLevel.h"
namespace OrderBook {

    std::tuple<EventStatus, std::list<Order>::iterator> PriceLevel::submitOrder(orderID ID, unsigned int newQuantity, unsigned int newPrice) {
        if (newPrice  != this->price) {
            return {EventStatus::Illegal, orderList.end()};
        }

        orderList.emplace_back(ID, newQuantity, newPrice);
        this->quantity += newQuantity;
        return {EventStatus::Success, --orderList.end()};
    }

    EventStatus PriceLevel::updateOrder(std::list<Order>::iterator order, unsigned int newQuantity, unsigned int newPrice) {
        if (newPrice  != this->price) {
            return EventStatus::Illegal;
        }

        if (!newQuantity) {
            return cancelOrder(order);
        }

        this->quantity += newQuantity - order->quantity;
        order->quantity = newQuantity;
        order->price = newPrice;
        return EventStatus::Success;
    }

    EventStatus PriceLevel::cancelOrder(std::list<Order>::iterator order) {
        quantity -= order->quantity;
        orderList.erase(order);
        return EventStatus::Success;
    }

    EventStatus PriceLevel::executeOrder(std::list<Order>::iterator order, unsigned int executeQuantity) {
        if (order->quantity < executeQuantity) {
            return EventStatus::Illegal;
        }

        quantity -= executeQuantity;
        order->quantity -= executeQuantity;
        if (!order->quantity) {
            orderList.erase(order);
        }

        // TODO add log
        return EventStatus::Success;
    }
}
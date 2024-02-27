#include "PriceLevel.h"
namespace OrderBook {

    std::tuple<EventStatus, std::list<Order>::iterator> PriceLevel::submitOrder(orderID ID, int quantity, int price) {
        return {};
    }

    EventStatus PriceLevel::updateOrder(std::list<Order>::iterator order, int quantity, int price) {
        return fail;
    }

    EventStatus PriceLevel::deleteOrder(std::list<Order>::iterator order) {
        return fail;
    }

    EventStatus PriceLevel::executeOrder(std::list<Order>::iterator order) {
        return fail;
    }
}
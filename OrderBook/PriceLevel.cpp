#include "PriceLevel.h"
namespace OrderBook {

    std::tuple<EventStatus, std::list<Order>::iterator> PriceLevel::submitOrder(orderID ID, int quantity, int price) {
        return {};
    }

    EventStatus PriceLevel::updateOrder(Order *order, int quantity, int price) {
        return fail;
    }

    EventStatus PriceLevel::deleteOrder(Order *order) {
        return fail;
    }

    EventStatus PriceLevel::executeOrder(Order *order) {
        return fail;
    }
}
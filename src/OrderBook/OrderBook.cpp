#include "OrderBook.h"
#include "../Event/TradeEvent.h"
namespace OrderBook {
    EventStatus OrderBook::submitOrder(orderID ID, unsigned int quantity, unsigned int price, TradeDirection tradeDirection) {
        EventStatus status;
        if (tradeDirection == TradeDirection::Buy) {
            status = buyBook.submitOrder(ID, quantity, price);
        } else {
            status = sellBook.submitOrder(ID, quantity, price);
        }

        if (status == EventStatus::Success) {
            updateBestBidAndOffer();
        }

        return status;
    }

    EventStatus OrderBook::updateOrder(orderID ID, unsigned int quantity, unsigned int price, TradeDirection tradeDirection) {
        EventStatus status;
        if (tradeDirection == TradeDirection::Buy) {
            status = buyBook.updateOrder(ID, quantity, price);
        } else {
            status = sellBook.updateOrder(ID, quantity, price);
        }

        if (status == EventStatus::Success) {
            updateBestBidAndOffer();
        }

        return status;
    }

    EventStatus OrderBook::executeOrder(orderID ID, unsigned int quantity, TradeDirection tradeDirection) {
        EventStatus status;
        if (tradeDirection == TradeDirection::Buy) {
            status = buyBook.executeOrder(ID, quantity);
        } else {
            status = sellBook.executeOrder(ID, quantity);
        }

        if (status == EventStatus::Success) {
            updateBestBidAndOffer();
        }

        return status;
    }
    EventStatus OrderBook::deleteOrder(orderID ID, TradeDirection tradeDirection) {
        EventStatus status;
        if (tradeDirection == TradeDirection::Buy) {
            status = buyBook.deleteOrder(ID);
        } else {
            status = sellBook.deleteOrder(ID);
        }

        if (status == EventStatus::Success) {
            updateBestBidAndOffer();
        }

        return status;
    }

    void OrderBook::updateBestBidAndOffer() {
        bestBid = buyBook.getBestPriceLevel();
        bestOffer = sellBook.getBestPriceLevel();
    }


}// namespace OrderBook
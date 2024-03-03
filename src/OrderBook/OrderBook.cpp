#include "OrderBook.h"
#include "../Event/TradeEvent.h"
namespace OrderBook {
    EventStatus orderBookTest::submitOrder(orderID ID, unsigned int quantity, unsigned int price, TradeDirection tradeDirection) {
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

    EventStatus orderBookTest::updateOrder(orderID ID, unsigned int quantity, unsigned int price, TradeDirection tradeDirection) {
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

    EventStatus orderBookTest::executeOrder(orderID ID, unsigned int quantity, TradeDirection tradeDirection) {
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
    EventStatus orderBookTest::deleteOrder(orderID ID, TradeDirection tradeDirection) {
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

    void orderBookTest::updateBestBidAndOffer() {
        bestBid = buyBook.GetBestPriceLevel();
        bestOffer = sellBook.GetBestPriceLevel();
    }


}// namespace OrderBook
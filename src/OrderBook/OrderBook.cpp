#include "OrderBook.h"
#include "../Event/TradeEvent.h"
namespace OrderBook {
    EventStatus orderBookTest::submitOrder(orderID ID, int quanatity, int price, TradeDirection tradeDirection) {
        EventStatus status;
        if (tradeDirection == TradeDirection::Buy) {
            status = buyBook.submitOrder(ID, quanatity, price);
        } else {
            status = sellBook.submitOrder(ID, quanatity, price);
        }

        if (status == EventStatus::Success) {
            updateBestBidAndOffer();
        }

        return status;
    }

    EventStatus orderBookTest::updateOrder(orderID ID, int quanatity, int price, TradeDirection tradeDirection) {
        EventStatus status;
        if (tradeDirection == TradeDirection::Buy) {
            status = buyBook.updateOrder(ID, quanatity, price);
        } else {
            status = sellBook.updateOrder(ID, quanatity, price);
        }

        if (status == EventStatus::Success) {
            updateBestBidAndOffer();
        }

        return status;
    }

    EventStatus orderBookTest::executeOrder(orderID ID, int quanatity, TradeDirection tradeDirection) {
        EventStatus status;
        if (tradeDirection == TradeDirection::Buy) {
            status = buyBook.executeOrder(ID, quanatity);
        } else {
            status = sellBook.executeOrder(ID, quanatity);
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
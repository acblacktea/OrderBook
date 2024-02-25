#include "OrderBook.h"
#include "../Event/TradeEvent.h"
namespace OrderBook {

    EventStatus OrderBook::submitOrder(orderID ID, int quanatity, int price, int tradeDirection) {
        EventStatus status;
        if (tradeDirection == 1) {
            status = buyBook.submitOrder(ID, quanatity, price);
        } else {
            status = sellBook.submitOrder(ID, quanatity, price);
        }

        if (status == success) {
            updateBestBidAndOffer();
        }

        return status;
    }

    EventStatus OrderBook::updateOrder(orderID ID, int quanatity, int price, int tradeDirection) {
        EventStatus status;
        if (tradeDirection == 1) {
            status = buyBook.updateOrder(ID, quanatity, price);
        } else {
            status = sellBook.updateOrder(ID, quanatity, price);
        }

        if (status == success) {

        }

        return status;
    }

    EventStatus OrderBook::executeOrder(orderID ID, int quanatity, int tradeDirection) {
        EventStatus status;
        if (tradeDirection == 1) {
            status = buyBook.executeOrder(ID, quanatity);
        } else {
            status = sellBook.executeOrder(ID, quanatity);
        }

        if (status == success) {

        }

        return status;
    }
    EventStatus OrderBook::deleteOrder(orderID ID, int tradeDirection) {
        EventStatus status;
        if (tradeDirection == 1) {
            status = buyBook.deleteOrder(ID);
        } else {
            status = sellBook.deleteOrder(ID);
        }

        if (status == success) {

        }

        return status;
    }

    void OrderBook::updateBestBidAndOffer() {
        bestBid = buyBook.GetBestPriceLevel();
        bestOffer = sellBook.GetBestPriceLevel();
    }


}// namespace OrderBook
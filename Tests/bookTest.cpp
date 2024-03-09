#include <OrderBook/Book.h>
#include <gtest/gtest.h>

class BookHelperTest : public testing::Test {
public:
    template<OrderBook::TradeDirection direction>
    static OrderBook::Book<direction> buildBook() {
        OrderBook::Book<direction> book;
        return book;
    };

    static void test_singleOrder(OrderBook::Book<OrderBook::TradeDirection::Sell> &sellBook,
                                 int orderID,
                                 int quantity,
                                 int price,
                                 int totalQuantity,
                                 int bestPrice,
                                 int bestPriceQuantity,
                                 int priceLevelLength,
                                 int priceQuantity) {
        EXPECT_EQ(sellBook.GetTotalQuantity(), totalQuantity);

        auto bestPriceLevel = sellBook.GetBestPriceLevel();
        EXPECT_EQ(bestPriceLevel.price, bestPrice);
        EXPECT_EQ(bestPriceLevel.quantity, bestPriceQuantity);


        auto order = sellBook.GetOrderByOrderID(orderID);
        if (quantity != 0) {
            EXPECT_NE(order, nullptr);
            EXPECT_EQ(order->price, price);
            EXPECT_EQ(order->quantity, quantity);
        } else {
            EXPECT_EQ(order, nullptr);
        }

        auto priceLevel = sellBook.GetPriceLevelByOrderID(orderID);
        if (quantity != 0) {
            EXPECT_NE(priceLevel, nullptr);
            EXPECT_EQ(priceLevel->getPrice(), price);
            EXPECT_EQ(priceLevel->getOrderLength(), priceLevelLength);
            EXPECT_EQ(priceLevel->getQuantity(), priceQuantity);
        } else {
            EXPECT_EQ(priceLevel, nullptr);
        }


        priceLevel = sellBook.GetPriceLevelByPrice(price);
        if (priceLevelLength != 0) {
            EXPECT_NE(priceLevel, nullptr);
            EXPECT_EQ(priceLevel->getPrice(), price);
            EXPECT_EQ(priceLevel->getOrderLength(), priceLevelLength);
            EXPECT_EQ(priceLevel->getQuantity(), priceQuantity);
        } else {
            EXPECT_EQ(priceLevel, nullptr);
        }
    }
};

TEST(BookHelperTest, test_add_order) {
    OrderBook::Book<OrderBook::TradeDirection::Sell> sellBook;
    sellBook.submitOrder(1, 12, 2);
    BookHelperTest::test_singleOrder(sellBook, 1, 12, 2, 12, 2, 12, 1, 12);
    sellBook.submitOrder(2, 13, 2);
    BookHelperTest::test_singleOrder(sellBook, 2, 13, 2, 25, 2, 25, 2, 25);
    sellBook.submitOrder(3, 14, 2);
    BookHelperTest::test_singleOrder(sellBook, 3, 14, 2, 39, 2, 39, 3, 39);

    sellBook.submitOrder(4, 12, 1);
    BookHelperTest::test_singleOrder(sellBook, 4, 12, 1, 51, 2, 39, 1, 12);
    sellBook.submitOrder(5, 13, 1);
    BookHelperTest::test_singleOrder(sellBook, 5, 13, 1, 64, 2, 39, 2, 25);
    sellBook.submitOrder(6, 14, 1);
    BookHelperTest::test_singleOrder(sellBook, 6, 14, 1, 78, 2, 39, 3, 39);

    sellBook.submitOrder(7, 12, 3);
    BookHelperTest::test_singleOrder(sellBook, 7, 12, 3, 90, 3, 12, 1, 12);
    sellBook.submitOrder(8, 13, 3);
    BookHelperTest::test_singleOrder(sellBook, 8, 13, 3, 103, 3, 25, 2, 25);
    sellBook.submitOrder(9, 14, 3);
    BookHelperTest::test_singleOrder(sellBook, 9, 14, 3, 117, 3, 39, 3, 39);
}

TEST(BookHelperTest, test_execute_order_simple) {
    OrderBook::Book<OrderBook::TradeDirection::Sell> sellBook;
    sellBook.submitOrder(1, 12, 2);
    sellBook.submitOrder(2, 13, 2);
    sellBook.submitOrder(3, 14, 2);

    sellBook.executeOrder(1, 6);
    BookHelperTest::test_singleOrder(sellBook, 1, 6, 2, 33, 2, 33, 3, 33);
    sellBook.executeOrder(1, 6);
    BookHelperTest::test_singleOrder(sellBook, 1, 0, 2, 27, 2, 27, 2, 27);
    sellBook.executeOrder(2, 6);
    BookHelperTest::test_singleOrder(sellBook, 2, 7, 2, 21, 2, 21, 2, 21);
    sellBook.executeOrder(2, 7);
    BookHelperTest::test_singleOrder(sellBook, 2, 0, 2, 14, 2, 14, 1, 14);
    sellBook.executeOrder(3, 6);
    BookHelperTest::test_singleOrder(sellBook, 3, 8, 2, 8, 2, 8, 1, 8);
    sellBook.executeOrder(3, 8);
    BookHelperTest::test_singleOrder(sellBook, 3, 0, 2, 0, -1, -1, 0, 0);
}

TEST(BookHelperTest, test_execute_order) {
    OrderBook::Book<OrderBook::TradeDirection::Sell> sellBook;
    sellBook.submitOrder(1, 12, 2);
    sellBook.submitOrder(2, 13, 2);
    sellBook.submitOrder(3, 14, 2);
    sellBook.submitOrder(4, 12, 1);
    sellBook.submitOrder(5, 13, 1);
    sellBook.submitOrder(6, 14, 1);
    sellBook.submitOrder(7, 12, 3);
    sellBook.submitOrder(8, 13, 3);
    sellBook.submitOrder(9, 14, 3);

    sellBook.executeOrder(1, 6);
    BookHelperTest::test_singleOrder(sellBook, 1, 6, 2, 111, 3, 39, 3, 33);
    sellBook.executeOrder(1, 6);
    BookHelperTest::test_singleOrder(sellBook, 1, 0, 2, 105, 3, 39, 2, 27);
    sellBook.executeOrder(2, 6);
    BookHelperTest::test_singleOrder(sellBook, 2, 7, 2, 99, 3, 39, 2, 21);
    sellBook.executeOrder(2, 7);
    BookHelperTest::test_singleOrder(sellBook, 2, 0, 2, 92, 3, 39, 1, 14);
    sellBook.executeOrder(3, 6);
    BookHelperTest::test_singleOrder(sellBook, 3, 8, 2, 86, 3, 39, 1, 8);
    sellBook.executeOrder(3, 8);
    BookHelperTest::test_singleOrder(sellBook, 3, 0, 2, 78, 3, 39, 0, 0);

    sellBook.executeOrder(7, 6);
    BookHelperTest::test_singleOrder(sellBook, 7, 6, 3, 72, 3, 33, 3, 33);
    sellBook.executeOrder(7, 6);
    BookHelperTest::test_singleOrder(sellBook, 7, 0, 3, 66, 3, 27, 2, 27);
    sellBook.executeOrder(8, 6);
    BookHelperTest::test_singleOrder(sellBook, 8, 7, 3, 60, 3, 21, 2, 21);
    sellBook.executeOrder(8, 7);
    BookHelperTest::test_singleOrder(sellBook, 8, 0, 3, 53, 3, 14, 1, 14);
    sellBook.executeOrder(9, 6);
    BookHelperTest::test_singleOrder(sellBook, 9, 8, 3, 47, 3, 8, 1, 8);
    sellBook.executeOrder(9, 8);
    BookHelperTest::test_singleOrder(sellBook, 9, 0, 3, 39, 1, 39, 0, 0);

    sellBook.executeOrder(4, 6);
    BookHelperTest::test_singleOrder(sellBook, 4, 6, 1, 33, 1, 33, 3, 33);
    sellBook.executeOrder(4, 6);
    BookHelperTest::test_singleOrder(sellBook, 4, 0, 1, 27, 1, 27, 2, 27);
    sellBook.executeOrder(5, 6);
    BookHelperTest::test_singleOrder(sellBook, 5, 7, 1, 21, 1, 21, 2, 21);
    sellBook.executeOrder(5, 7);
    BookHelperTest::test_singleOrder(sellBook, 5, 0, 1, 14, 1, 14, 1, 14);
    sellBook.executeOrder(6, 6);
    BookHelperTest::test_singleOrder(sellBook, 6, 8, 1, 8, 1, 8, 1, 8);
    sellBook.executeOrder(6, 8);
    BookHelperTest::test_singleOrder(sellBook, 6, 0, 1, 0, -1, -1, 0, 0);
}


TEST(BookHelperTest, test_delete_order) {
    OrderBook::Book<OrderBook::TradeDirection::Sell> sellBook;
    sellBook.submitOrder(1, 12, 2);
    sellBook.submitOrder(2, 13, 2);
    sellBook.submitOrder(3, 14, 2);
    sellBook.submitOrder(4, 12, 1);
    sellBook.submitOrder(5, 13, 1);
    sellBook.submitOrder(6, 14, 1);
    sellBook.submitOrder(7, 12, 3);
    sellBook.submitOrder(8, 13, 3);
    sellBook.submitOrder(9, 14, 3);

    sellBook.deleteOrder(1);
    BookHelperTest::test_singleOrder(sellBook, 1, 0, 2, 105, 3, 39, 2, 27);
    sellBook.deleteOrder(2);
    BookHelperTest::test_singleOrder(sellBook, 2, 0, 2, 92, 3, 39, 1, 14);
    sellBook.deleteOrder(3);
    BookHelperTest::test_singleOrder(sellBook, 3, 0, 2, 78, 3, 39, 0, 0);

    sellBook.deleteOrder(7);
    BookHelperTest::test_singleOrder(sellBook, 7, 0, 3, 66, 3, 27, 2, 27);
    sellBook.deleteOrder(8);
    BookHelperTest::test_singleOrder(sellBook, 8, 0, 3, 53, 3, 14, 1, 14);
    sellBook.deleteOrder(9);
    BookHelperTest::test_singleOrder(sellBook, 9, 0, 3, 39, 1, 39, 0, 0);

    sellBook.deleteOrder(4);
    BookHelperTest::test_singleOrder(sellBook, 4, 0, 1, 27, 1, 27, 2, 27);
    sellBook.deleteOrder(5);
    BookHelperTest::test_singleOrder(sellBook, 5, 0, 1, 14, 1, 14, 1, 14);
    sellBook.deleteOrder(6);
    BookHelperTest::test_singleOrder(sellBook, 6, 0, 1, 0, -1, -1, 0, 0);
}


TEST(BookHelperTest, test_update_order) {
    OrderBook::Book<OrderBook::TradeDirection::Sell> sellBook;
    sellBook.submitOrder(1, 12, 2);
    sellBook.submitOrder(2, 13, 2);
    sellBook.submitOrder(3, 14, 2);
    sellBook.submitOrder(4, 12, 1);
    sellBook.submitOrder(5, 13, 1);
    sellBook.submitOrder(6, 14, 1);
    sellBook.submitOrder(7, 12, 3);
    sellBook.submitOrder(8, 13, 3);
    sellBook.submitOrder(9, 14, 3);


    sellBook.updateOrder(9, 7, 3);
    BookHelperTest::test_singleOrder(sellBook, 9, 7, 3, 110, 3, 32, 3, 32);

    sellBook.updateOrder(9, 0, 3);
    BookHelperTest::test_singleOrder(sellBook, 9, 0, 3, 103, 3, 25, 2, 25);

    sellBook.updateOrder(8, 15, 2);
    BookHelperTest::test_singleOrder(sellBook, 8, 15, 2, 105, 3, 12, 4, 54);
    auto priceLevel = sellBook.GetPriceLevelByPrice(3);
    EXPECT_EQ(priceLevel->getPrice(), 3);
    EXPECT_EQ(priceLevel->getOrderLength(), 1);
    EXPECT_EQ(priceLevel->getQuantity(), 12);

    sellBook.updateOrder(7, 3, 1);
    BookHelperTest::test_singleOrder(sellBook, 7, 3, 1, 96, 2, 54, 4, 42);
    priceLevel = sellBook.GetPriceLevelByPrice(3);
    EXPECT_EQ(priceLevel, nullptr);
}
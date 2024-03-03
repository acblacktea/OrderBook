#include <gtest/gtest.h>
#include <OrderBook/PriceLevel.h>

TEST(priceLevel, test_add_order) {
   auto priveLevel = OrderBook::PriceLevel(5 /*price*/);
   auto [eventStatus1, iter1] = priveLevel.submitOrder(1, 233, 5);
   EXPECT_EQ(eventStatus1, OrderBook::EventStatus::Success);
   EXPECT_EQ(iter1->orderID, 1);
   EXPECT_EQ(iter1->price, 5);
   EXPECT_EQ(iter1->quantity, 233);

   auto [eventStatus2, iter2] = priveLevel.submitOrder(2, 2333, 5);
   EXPECT_EQ(eventStatus2, OrderBook::EventStatus::Success);
   EXPECT_EQ(iter2->orderID, 2);
   EXPECT_EQ(iter2->price, 5);
   EXPECT_EQ(iter2->quantity, 2333);

   auto [eventStatus3, iter3] = priveLevel.submitOrder(3, 23333, 5);
   EXPECT_EQ(eventStatus3, OrderBook::EventStatus::Success);
   EXPECT_EQ(iter3->orderID, 3);
   EXPECT_EQ(iter3->price, 5);
   EXPECT_EQ(iter3->quantity, 23333);

   EXPECT_EQ(priveLevel.getQuantity(), 25899);
   EXPECT_EQ(priveLevel.getPrice(), 5);
}

TEST(priceLevel, test_update_order) {
   auto priveLevel = OrderBook::PriceLevel(5 /*price*/);
   auto [eventStatus1, iter1] = priveLevel.submitOrder(1, 233, 5);
   auto [eventStatus2, iter2] = priveLevel.submitOrder(2, 2333, 5);
   auto [eventStatus3, iter3] = priveLevel.submitOrder(3, 23333, 5);
   auto [eventStatus4, iter4] = priveLevel.submitOrder(3, 233333, 5);

   EXPECT_EQ(priveLevel.getQuantity(), 259232);
   EXPECT_EQ(priveLevel.getPrice(), 5);

   priveLevel.updateOrder(iter2, 2332, 5);

   EXPECT_EQ(iter2->quantity, 2332);
   EXPECT_EQ(iter2->price, 5);
   EXPECT_EQ(priveLevel.getQuantity(), 259231);
   EXPECT_EQ(priveLevel.getPrice(), 5);
}

TEST(priceLevel, test_cancel_order) {
   auto priveLevel = OrderBook::PriceLevel(5 /*price*/);
   auto [eventStatus1, iter1] = priveLevel.submitOrder(1, 233, 5);
   auto [eventStatus2, iter2] = priveLevel.submitOrder(2, 2333, 5);
   auto [eventStatus3, iter3] = priveLevel.submitOrder(3, 23333, 5);
   auto [eventStatus4, iter4] = priveLevel.submitOrder(3, 233333, 5);


   priveLevel.cancelOrder(iter2);
   EXPECT_EQ(priveLevel.getQuantity(), 256899);
   EXPECT_EQ(priveLevel.getPrice(), 5);
   EXPECT_EQ(priveLevel.getOrderLength(), 3);

   priveLevel.cancelOrder(iter3);
   EXPECT_EQ(priveLevel.getQuantity(), 233566);
   EXPECT_EQ(priveLevel.getPrice(), 5);
   EXPECT_EQ(priveLevel.getOrderLength(), 2);

   priveLevel.cancelOrder(iter1);
   EXPECT_EQ(priveLevel.getQuantity(), 233333);
   EXPECT_EQ(priveLevel.getPrice(), 5);
   EXPECT_EQ(priveLevel.getOrderLength(), 1);

   priveLevel.cancelOrder(iter4);
   EXPECT_EQ(priveLevel.getQuantity(), 0);
   EXPECT_EQ(priveLevel.getPrice(), 5);
   EXPECT_EQ(priveLevel.getOrderLength(), 0);
}


TEST(priceLevel, test_execute_order) {
   auto priveLevel = OrderBook::PriceLevel(5 /*price*/);
   auto [eventStatus1, iter1] = priveLevel.submitOrder(1, 233, 5);
   auto [eventStatus2, iter2] = priveLevel.submitOrder(2, 2333, 5);
   auto [eventStatus3, iter3] = priveLevel.submitOrder(3, 23333, 5);
   auto [eventStatus4, iter4] = priveLevel.submitOrder(3, 233333, 5);


   priveLevel.executeOrder(iter2);
   EXPECT_EQ(priveLevel.getQuantity(), 256899);
   EXPECT_EQ(priveLevel.getPrice(), 5);
   EXPECT_EQ(priveLevel.getOrderLength(), 3);

   priveLevel.executeOrder(iter3);
   EXPECT_EQ(priveLevel.getQuantity(), 233566);
   EXPECT_EQ(priveLevel.getPrice(), 5);
   EXPECT_EQ(priveLevel.getOrderLength(), 2);

   priveLevel.executeOrder(iter1);
   EXPECT_EQ(priveLevel.getQuantity(), 233333);
   EXPECT_EQ(priveLevel.getPrice(), 5);
   EXPECT_EQ(priveLevel.getOrderLength(), 1);

   priveLevel.executeOrder(iter4);
   EXPECT_EQ(priveLevel.getQuantity(), 0);
   EXPECT_EQ(priveLevel.getPrice(), 5);
   EXPECT_EQ(priveLevel.getOrderLength(), 0);
}
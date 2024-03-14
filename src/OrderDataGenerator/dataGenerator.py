import json
import random
import sys
import time
from enum import Enum
from sortedcontainers import SortedList, SortedSet, SortedDict


class OrderType(Enum):
    Submit = 1
    Cancellation = 2
    Deletion = 3
    LimitOrderExecution = 4


class TradeDirection(Enum):
    Buy = 1
    Sell = -1


class Order:
    def __init__(self, price, quantity, time, orderID):
        self.orderID = orderID
        self.price = price
        self.quantity = quantity
        self.time = time


buySet = SortedSet([], key=lambda x: (x.price, x.time))
sellSet = SortedSet([], key=lambda x: (-x.price, x.time))


class OrderGenerator:
    def __init__(self, bestBuyPrice, bestSellPrice, orderSize):
        self.name = "OrderGenerator"
        self.bestBuyPrice = bestBuyPrice
        self.bestSellPrice = bestSellPrice
        self.orderSize = orderSize
        self.orderID = 0

    def generateOrder(self):
        preMarketOrderSize = self.orderSize / 10
        self.initPreMarketOrders(preMarketOrderSize)
        inMarketOrderSize = self.orderSize - preMarketOrderSize

        for i in range(inMarketOrderSize):
            randVal = random.randrange(100)
            if randVal <= 93:
                self.__submitOrder()
            elif 94 <= randVal <= 96:
                self.__updateOrder()
            else:
                self.__deleteOrder()

        print("7,0,0,0,0")

    def initPreMarketOrders(self, preMarketOrderSize):
        buyQuantity = self.__generateQuantity()
        sellQuantity = self.__generateQuantity()
        order = Order(self.bestBuyPrice, buyQuantity, time.time() * 1000, self.orderID)
        buySet.add(order)
        print("%d,%d,%d,%d,%d,%d",
              order.time,
              OrderType.Submit.value,
              order.orderID,
              order.price,
              buyQuantity,
              TradeDirection.Buy.value)

        self.orderID += 1

        order = Order(self.bestSellPrice, sellQuantity, time.time() * 1000, self.orderID)
        sellSet.add(order)
        print("%d,%d,%d,%d,%d,%d",
              time.time() * 1000,
              OrderType.Submit.value,
              self.orderID,
              self.bestSellPrice,
              sellQuantity,
              TradeDirection.Sell.value)
        self.orderID += 1

        for i in range(preMarketOrderSize):
            tradeDirection = TradeDirection.Buy.value
            randVal = random.randrange(2)
            quantity = self.__generateQuantity()
            rightValue = self.bestBuyPrice
            leftValue = max(0, self.bestBuyPrice - 200)
            if randVal == 1:
                tradeDirection = TradeDirection.Sell.value
                leftValue = self.bestSellPrice
                rightValue = self.bestBuyPrice + 200

            price = random.randrange(leftValue, rightValue + 1)

            order = Order(price, quantity, time.time() * 1000, self.orderID)
            if tradeDirection == 1:
                buySet.add(order)
            else:
                sellSet.add(order)
            print("%d,%d,%d,%d,%d,%d",
                  time.time() * 1000,
                  OrderType.Submit.value,
                  self.orderID,
                  price,
                  quantity,
                  tradeDirection)
            self.orderID += 1

    def __submitOrder(self):
        tradeDirection = TradeDirection.Buy.value
        randVal = random.randrange(2)
        quantity = self.__generateQuantity()
        if randVal == 1:
            tradeDirection = TradeDirection.Sell.value

        price = random.randrange(self.bestBuyPrice - 20, self.bestSellPrice + 2)
        if tradeDirection == TradeDirection.Sell.value:
            price = random.randrange(self.bestBuyPrice - 2, self.bestSellPrice + 20)

        order = Order(price, quantity, time.time() * 1000, self.orderID)
        if tradeDirection == 1:
            buySet.add(order)
        else:
            sellSet.add(order)
        print("%d,%d,%d,%d,%d,%d",
              time.time() * 1000,
              OrderType.Submit.value,
              self.orderID,
              price,
              quantity,
              tradeDirection)
        self.orderID += 1
        self.__updateBBO()
        self.__scanTradeOrders()

    def __updateOrder(self):
        #todo
        randVal = random.randrange(2)
        randVal2 = random.randrange(2)
        newPrice = 0
        newQuantity = 0
        orderID = 0
        if randVal == 1:
            index = random.randrange(len(buySet))
            if randVal2 == 1:
                left = max(buySet[index].price - 20, 1)
                right = buySet[index].price + 20
                newPrice = random.randrange(left, right + 1)
                newQuantity = buySet[index].quantity
                order = Order(newPrice, newQuantity, time.time() * 1000, buySet[index].orderID)
                buySet.discard(buySet[index])
                buySet.add(order)

            else:
                left = max(buySet[index].quantity - 1000, 1)
                right = buySet[index].quantity
                newPrice = buySet[index].price
                newQuantity = random.randrange(left, right - 1)
                order = Order(newPrice, newQuantity, time.time() * 1000, buySet[index].orderID)
                buySet.discard(buySet[index])
                buySet.add(order)

            print("%d,%d,%d,%d,%d,%d",
                  time.time() * 1000,
                  OrderType.Cancellation.value,
                  orderID,
                  newPrice,
                  newQuantity,
                  TradeDirection.Buy.value)
        else:
            index = random.randrange(len(sellSet))
            if randVal2 == 1:
                left = max(sellSet[index].price - 20, 1)
                right = sellSet[index].price + 20
                newPrice = random.randrange(left, right + 1)
                newQuantity = sellSet[index].quantity
                order = Order(newPrice, newQuantity, time.time() * 1000, sellSet[index].orderID)
                sellSet.discard(sellSet[index])
                sellSet.add(order)

            else:
                left = max(sellSet[index].quantity - 1000, 1)
                right = sellSet[index].quantity
                newPrice = sellSet[index].price
                newQuantity = random.randrange(left, right - 1)
                order = Order(newPrice, newQuantity, time.time() * 1000, sellSet[index].orderID)
                sellSet.discard(buySet[index])
                sellSet.add(order)

            print("%d,%d,%d,%d,%d,%d",
                  time.time() * 1000,
                  OrderType.Cancellation.value,
                  orderID,
                  newPrice,
                  newQuantity,
                  TradeDirection.Buy.value)
        self.__updateBBO()
        self.__scanTradeOrders()

    def __deleteOrder(self):
        randVal = random.randrange(2)
        if randVal == 1:
            index = random.randrange(len(buySet))
            order = buySet[index]
            buySet.discard(order)
            print("%d,%d,%d,%d,%d,%d",
                  time.time() * 1000,
                  OrderType.Deletion.value,
                  self.orderID,
                  order.price,
                  order.quantity,
                  TradeDirection.Buy.value)
        else:
            index = random.randrange(len(sellSet))
            order = sellSet[index]
            sellSet.discard(order)
            print("%d,%d,%d,%d,%d,%d",
                  time.time() * 1000,
                  OrderType.Deletion.value,
                  self.orderID,
                  order.price,
                  order.quantity,
                  TradeDirection.Sell.value)

        self.__updateBBO()
        self.__scanTradeOrders()

    def __scanTradeOrders(self):
        while len(buySet) and len(sellSet) and self.bestBuyPrice >= self.bestSellPrice:
            buyOrder = buySet[0]
            sellOrder = sellSet[0]
            buySet.discard(buySet[0])
            sellSet.discard(sellSet[0])
            quantity = min(buyOrder.quantity, sellOrder.quantity)
            print("%d,%d,%d,%d,%d,%d",
                  time.time() * 1000,
                  OrderType.LimitOrderExecution.value,
                  buyOrder.orderID,
                  buyOrder.price,
                  quantity,
                  TradeDirection.Buy.value)

            print("%d,%d,%d,%d,%d,%d",
                  time.time() * 1000,
                  OrderType.LimitOrderExecution.value,
                  sellOrder.orderID,
                  sellOrder.price,
                  quantity,
                  TradeDirection.Sell.value)

            buyOrder.quantity -= quantity
            sellOrder.quantity -= quantity

            if buyOrder.quantity > 0:
                buySet.add(buyOrder)

            if sellOrder.quantity > 0:
                sellSet.add(sellOrder)

            self.__updateBBO()


    def __updateBBO(self):
        self.bestBuyPrice = buySet[0].price
        self.bestSellPrice = sellSet[0].price

    def __generateQuantity(self):
        return random.randrange(99999) + 1


def test():
    buySet.add(Order(5, 1, 1, 1))
    buySet.add(Order(12, 1, 2, 2))
    buySet.add(Order(12, 1, 3, 3))
    print("%d %d %d" % (buySet[0].price, buySet[0].quantity, buySet[0].time))
    print("%d %d %d" % (buySet[1].price, buySet[1].quantity, buySet[1].time))
    print("%d %d %d" % (buySet[2].price, buySet[2].quantity, buySet[2].time))

    print()

    sellSet.add(Order(5, 1, 1, 4))
    sellSet.add(Order(12, 1, 2, 5))
    sellSet.add(Order(12, 1, 3, 6))
    print("%d %d %d" % (sellSet[0].price, sellSet[0].quantity, sellSet[0].time))
    print("%d %d %d" % (sellSet[1].price, sellSet[1].quantity, sellSet[1].time))
    print("%d %d %d" % (sellSet[2].price, sellSet[2].quantity, sellSet[2].time))

    sellSet[0].price = 100
    print("%d %d %d" % (sellSet[0].price, sellSet[0].quantity, sellSet[0].time))
    sellSet.discard(sellSet[0])
    print(len(sellSet))


if __name__ == '__main__':
    # test()
    print("price: ", sys.argv[0])
    print("quantity: ", sys.argv[1])
    print("time: ", sys.argv[2])
    OrderGenerator(sys.argv[0], sys.argv[1], sys.argv[2])

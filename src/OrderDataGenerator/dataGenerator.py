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
    def __init__(self, price, quantity, time):
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
        print("%d,%d,%d,%d,%d,%d",
              time.time() * 1000,
              OrderType.Submit.value,
              self.orderID,
              self.bestBuyPrice,
              buyQuantity,
              TradeDirection.Buy.value)

        self.orderID += 1
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
        if randVal == 1:
            index = random.randrange(len(buySet))
            order = buySet[index]
            buySet.discard(index)
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
            sellSet.discard(index)
            print("%d,%d,%d,%d,%d,%d",
                  time.time() * 1000,
                  OrderType.Deletion.value,
                  self.orderID,
                  order.price,
                  order.quantity,
                  TradeDirection.Sell.value)
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
        print("todo")

    def __updateBBO(self):
        print("todo")

    def __generateQuantity(self):
        return random.randrange(99999) + 1


def test():
    buySet.add(Order(5, 1, 1))
    buySet.add(Order(12, 1, 2))
    buySet.add(Order(12, 1, 3))
    print("%d %d %d" % (buySet[0].price, buySet[0].quantity, buySet[0].time))
    print("%d %d %d" % (buySet[1].price, buySet[1].quantity, buySet[1].time))
    print("%d %d %d" % (buySet[2].price, buySet[2].quantity, buySet[2].time))

    print()

    sellSet.add(Order(5, 1, 1))
    sellSet.add(Order(12, 1, 2))
    sellSet.add(Order(12, 1, 3))
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

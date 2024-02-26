#include "pch.h"
#include "CppUnitTest.h"
#include "../C++_Practice/order_book.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ordermatching;
using namespace std::chrono;

namespace ut_order_book {

	TEST_CLASS(ut_order_book) {
	public:

		TEST_METHOD(addOrder) {

			Order order0("SELL", 0, 3, 2, system_clock::now());
			Order order1("SELL", 1, 2, 3, system_clock::now());

			Order order2("BUY", 2, 3, 3, system_clock::now());
			Order order3("BUY", 3, 2, 3, system_clock::now());

			std::vector<Order> expected_ordering_sell = {
				order1, order0
			};

			std::vector<Order> expected_ordering_buy = {
				order2, order3
			};

			OrderBook sell_order_book("BTC");
			OrderBook buy_order_book("BTC");

			sell_order_book.addOrder(order0);
			sell_order_book.addOrder(order1);

			buy_order_book.addOrder(order2);
			buy_order_book.addOrder(order3);
				
			auto sell_price_bucket = sell_order_book.getOrders();
			auto buy_price_bucket = buy_order_book.getOrders();

			int i = 0;
			for (auto& price : sell_price_bucket) {
				
				for (auto& time_ordering : price.second) {

					auto& order = time_ordering.second;
					Assert::AreEqual(order.getOrderId(), expected_ordering_sell[i++].getOrderId());
				}
			}

			i = expected_ordering_buy.size() - 1;
			for (auto& price : buy_price_bucket) {

				for (auto& time_ordering : price.second) {

					auto& order = time_ordering.second;
					Assert::AreEqual(order.getOrderId(), expected_ordering_buy[i--].getOrderId());
				}
			}
		}

		TEST_METHOD(matchOrder) {

			//Order of parameters --> order_type, id, price, qty, time 
			Order order0("SELL", 0, 3, 2, system_clock::now());
			Order order1("SELL", 1, 2, 3, system_clock::now());

			Order order2("BUY", 2, 3, 3, system_clock::now());
			Order order3("BUY", 3, 2, 2, system_clock::now());

			OrderBook sell_order_book("BTC");
			OrderBook buy_order_book("BTC");

			Order received_order[]{
				order0, order2, order3, order1
			};
			
			Quantity qty_remaning[]{
				2, 1, 2, 0 
			};

			int i = 0;
			for (auto order : received_order) {

				if (order.getSide() == BUY) {

					sell_order_book.matchOrder(order);
					Assert::AreEqual(order.getQuantity(), qty_remaning[i++]);

					if (order.getQuantity())
						buy_order_book.addOrder(order);
				}
				else {

					buy_order_book.matchOrder(order);
					Assert::AreEqual(order.getQuantity(), qty_remaning[i++]);

					if (order.getQuantity())
						sell_order_book.addOrder(order);
				}
			}
		}


		TEST_METHOD(modifyOrder) {
			
			Order order0("SELL", 0, 3, 2, system_clock::now());
			Order order1("SELL", 1, 2, 3, system_clock::now());

			OrderBook sell_order_book("BTC");

			sell_order_book.addOrder(order0);
			sell_order_book.addOrder(order1);

			sell_order_book.modifyOrder(order0, 1, 1);
			sell_order_book.modifyOrder(order1, 0, 2);

			auto sell_price_bucket = sell_order_book.getOrders();
				
			std::vector<std::pair<Quantity, Price>> expected_output{
				std::make_pair(1, 1)
			};

			int i = 0;
			for (auto price : sell_price_bucket) {

				for (auto time_ordering : price.second) {

					auto& order = time_ordering.second;
					Assert::AreEqual(order.getQuantity(), expected_output[i].first);
					Assert::AreEqual(order.getPrice(), expected_output[i++].second);
				}
			}
		}
	};
}

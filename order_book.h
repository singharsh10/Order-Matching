#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <list>

#include "order.h"


namespace ordermatching {
	
	// Both sides will have their own order book for each symbol resepctively
	class OrderBook {
	private:
		std::string symbol;
		std::map <Price, std::map<std::chrono::time_point<std::chrono::system_clock>, Order>> price_bucket;

	public:
		std::map<Id, std::map<std::chrono::time_point<std::chrono::system_clock>, Order>::iterator> id_tracker;

		OrderBook(std::string sym) : symbol(sym) 
		{}

		void addOrder(Order&);
		void matchOrder(Order&);
		void modifyOrder(Order&, Quantity, Price);
	};

	void OrderBook::addOrder(Order& order) {

		Id order_id = order.getOrderId();
		price_bucket[order.getPrice()].emplace(order.getTime(), order);
		id_tracker[order.getOrderId()] = price_bucket[order.getPrice()].find(order.getTime());
	}

	// we will iterate over Buy side order book for respective symbol
	// the symbol of either side will call this function for the counter side order
	void OrderBook::matchOrder(Order& order) {

		if (order.getSide() == SELL) {

			// from higher to lower price
			for (auto price_it = price_bucket.rbegin(); price_it != price_bucket.rend() && (*price_it).first >= order.getPrice(); price_it++) {

				auto& time_map = (*price_it).second;
				std::vector <std::map<std::chrono::time_point<std::chrono::system_clock>, Order>::iterator> remove;

				for (auto time_it = time_map.begin(); time_it != time_map.end(); time_it++) {

					auto& buy_order = (*time_it).second;
					Quantity remaining = buy_order.getQuantity() - order.getQuantity();
					
					if (remaining > 0) {

						buy_order.changeQuantity(0);
						buy_order.changeQuantity(remaining);
					}
					else {

						order.changeQuantity(abs(remaining));
						std::cout << "BUY order with order id: " << buy_order.getOrderId() << " traded" << std::endl;

						Order::id_to_order.erase(buy_order.getOrderId());
						id_tracker.erase(buy_order.getOrderId());
						remove.push_back(time_it);
					}

					if (order.getQuantity() == 0)
						break;
				}

				while (!remove.empty()) {

					time_map.erase(remove.back());
					remove.pop_back();
				}
			}
		}
		else {

			// from lower to higher price
			for (auto price_it = price_bucket.begin(); price_it != price_bucket.end(); price_it++) {

				auto& time_map = (*price_it).second;
				std::vector <std::map<std::chrono::time_point<std::chrono::system_clock>, Order>::iterator> remove;

				for (auto time_it = time_map.begin(); time_it != time_map.end(); time_it++) {

					auto& sell_order = (*time_it).second;
					Quantity remaining = sell_order.getQuantity() - order.getQuantity();

					if (remaining > 0) {

						order.changeQuantity(0);
						sell_order.changeQuantity(remaining);
					}
					else {

						order.changeQuantity(abs(remaining));
						std::cout << "SELL order with order id: " << sell_order.getOrderId() << " traded" << std::endl;

						Order::id_to_order.erase(sell_order.getOrderId());
						id_tracker.erase(sell_order.getOrderId());
						remove.push_back(time_it);
					}

					if (order.getQuantity() == 0)
						break;
				}

				while (!remove.empty()) {

					time_map.erase(remove.back());
					remove.pop_back();
				}
			}

		}
	}

	// not changing the timestamp on modifying the order
	void OrderBook::modifyOrder(Order& order, Quantity new_qty, Price new_price) {

		if (new_qty == 0) {

			if (order.getSide() == SELL)
				std::cout << "SELL order with order id: " << order.getOrderId() << " traded" << std::endl;
			else
				std::cout << "BUY order with order id: " << order.getOrderId() << " traded" << std::endl;
			
			price_bucket[order.getPrice()].erase(id_tracker[order.getOrderId()]);
			// need to erase the iterator as well
			id_tracker.erase(order.getOrderId());
			Order::id_to_order.erase(order.getOrderId());
			return;
		}

		order.changeQuantity(new_qty);

		if (new_price == order.getPrice())
			return;

		price_bucket[order.getPrice()].erase(id_tracker[order.getOrderId()]);
		id_tracker.erase(order.getOrderId());

		order.changePrice(new_price);
		
		price_bucket[order.getPrice()].emplace(order.getTime(), order);
		id_tracker[order.getOrderId()] = price_bucket[order.getPrice()].find(order.getTime());
	}

} // namespace ordermatching

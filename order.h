#pragma once
#include <chrono>
#include <list>
#include <iostream>
#include <vector>
#include <string>


class Order {
private:
	float price;
	uint64_t qty;
	uint64_t order_id;
	std::chrono::time_point<std::chrono::system_clock> time;

public:
	Order(float price, uint64_t quantity, uint64_t id, std::chrono::time_point<std::chrono::system_clock> arrival_time)
		: price(price), qty(quantity), order_id(id), time(arrival_time)
	{}

	float getPrice() const;
	uint64_t getQuantity() const;
	uint64_t getOrderId() const;
	std::chrono::time_point<std::chrono::system_clock> getTime() const;
	void changeQuantity(uint64_t);
};

float Order::getPrice() const {

	return price;
}

uint64_t Order::getQuantity() const {

	return qty;
}

uint64_t Order::getOrderId() const {

	return order_id;
}

std::chrono::time_point<std::chrono::system_clock> Order::getTime() const {

	return time;
}

void Order::changeQuantity(uint64_t amount) {

	qty = amount;
}

class Symbol {
private:
	std::string symbol;
	std::list<Order> orders;

public:
	Symbol(std::string sym)
		: symbol(sym)
	{}

	void addSellOrder(Order&);
	void addBuyOrder(Order&);
	void executeSellOrder(Order&);
	void executeBuyOrder(Order&);
	void remove_orders(std::vector<std::list<Order>::iterator>&);
};

void Symbol::addSellOrder(Order& order) {

	if (orders.empty()) {

		orders.push_front(order);
		return;
	}

	for (auto it = orders.begin(); it != orders.end(); it++) {

		if (it->getPrice() > order.getPrice() ||
			it->getPrice() == order.getPrice() && it->getTime() > order.getTime()) {

			orders.insert(it, order);
			return;
		}
	}

	orders.insert(orders.end(), order);
}

void Symbol::addBuyOrder(Order& order) {

	if (orders.empty()) {

		orders.push_front(order);
		return;
	}

	for (auto it = orders.begin(); it != orders.end(); it++) {

		if (it->getPrice() < order.getPrice() ||
			(it->getPrice() == order.getPrice() && it->getTime() > order.getTime())) {

			orders.insert(it, order);
			return;
		}
	}

	orders.insert(orders.end(), order);
}

void Symbol::executeSellOrder(Order& order) {

	std::vector<std::list<Order>::iterator> orders_traded;
	for (auto it = orders.begin(); it != orders.end(); it++) {

		if (it->getPrice() >= order.getPrice()) {

			int64_t qty_remaining = it->getQuantity() - order.getQuantity();
			if (qty_remaining > 0) {

				order.changeQuantity(0);
				it->changeQuantity(qty_remaining);
			}
			else {

				order.changeQuantity(abs(qty_remaining));
				std::cout << "Order traded with Order ID: " << it->getOrderId() << std::endl;
				orders_traded.push_back(it);
			}

			if (order.getQuantity() == 0) {

				remove_orders(orders_traded);
				return;
			}
		}
	}

	remove_orders(orders_traded);
}

void Symbol::executeBuyOrder(Order& order) {

	std::vector<std::list<Order>::iterator> orders_traded;
	for (auto it = orders.begin(); it != orders.end(); it++) {

		if (it->getPrice() <= order.getPrice()) {

			int64_t qty_remaining = it->getQuantity() - order.getQuantity();
			if (qty_remaining > 0) {

				order.changeQuantity(0);
				it->changeQuantity(qty_remaining);
			}
			else {

				order.changeQuantity(abs(qty_remaining));
				std::cout << "Order traded with Order ID: " << it->getOrderId() << std::endl;
				orders_traded.push_back(it);
			}

			if (order.getQuantity() == 0) {

				remove_orders(orders_traded);
				return;
			}
		}
	}

	remove_orders(orders_traded);
}

void Symbol::remove_orders(std::vector<std::list<Order>::iterator>& orders_traded) {

	while (!orders_traded.empty()) {

		orders.erase(orders_traded.back());
		orders_traded.pop_back();
	}
}
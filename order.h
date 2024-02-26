#pragma once

#include <chrono>
#include <string>
#include <map>

#include "types.h"


namespace ordermatching {

	enum Side { BUY, SELL };

	class Order {
	private:
		Id order_id;
		Price price;
		Quantity qty;
		Side side;
		std::chrono::time_point<std::chrono::system_clock> time;

	public:
		inline static std::map<Id, Order> id_to_order;

		Order(std::string order_type, Id id, Price price, Quantity quantity, std::chrono::time_point<std::chrono::system_clock> arrival_time)
			: order_id(id), price(price), qty(quantity), time(arrival_time)
		{
			side = (order_type == "BUY") ? BUY : SELL;

			// id_to_order[id] = *this; calls default constructor and then copy assignment operator. use the one below
			Order::id_to_order.emplace(id, *this); // calls copy constructor only once. insert() creates a extra copy
		}

		Side getSide() const;
		Price getPrice() const;
		Quantity getQuantity() const;
		Id getOrderId() const;
		std::chrono::time_point<std::chrono::system_clock> getTime() const;

		void changePrice(Price);
		void changeQuantity(Quantity);
		
	};

	Side Order::getSide() const {

		return side;
	}

	Price Order::getPrice() const {

		return price;
	}

	Quantity Order::getQuantity() const {

		return qty;
	}

	Id Order::getOrderId() const {

		return order_id;
	}

	std::chrono::time_point<std::chrono::system_clock> Order::getTime() const {

		return time;
	}

	void Order::changePrice(Price new_price) {

		price = new_price;
	}

	void Order::changeQuantity(Quantity new_qty) {

		qty = new_qty;
	}

} // namespace ordermatching
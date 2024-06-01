#include "order_book.h"

using namespace std::chrono;
using namespace ordermatching;


void takeOrders() {
	
	std::map<std::string, OrderBook> buy_side_symbols;
	std::map<std::string, OrderBook> sell_side_symbols;

	while (true) {
		
		std::string order_type;
		Price price;
		Quantity qty;
		Id id;
		std::string symbol;
		time_point<system_clock> curr_time = system_clock::now();

		std::cin >> order_type >> symbol >> id;

		if (order_type == "CANCEL")	
			price = 0, qty = 0;
		else 
			std::cin >> price >> qty;

		/*
			TODO
			in case qty < 0 or price < 0 throw error.
		*/

		Order received_order = Order(order_type, id, price, qty, curr_time);

		if (order_type == "BUY") {

			auto symbol_orderbook = sell_side_symbols.find(symbol);
			auto have_symbol = buy_side_symbols.find(symbol);

			if (symbol_orderbook == sell_side_symbols.end()) {

				if (have_symbol == buy_side_symbols.end())
					have_symbol = buy_side_symbols.emplace(symbol, OrderBook(symbol)).first;
				
				/* can't use this buy_side_symbols[symbol].addOrder(received_order). It won't 
				 * compile as compiler needs default constructor even if key is present
				 * It does so to make sure in case the key is not present then a default object 
				 * can be constructed. Compiler does not know that this key will always exists. 
				 * Hence it complains.
				 */

				(*have_symbol).second.addOrder(received_order);
			}
			else {

				(*symbol_orderbook).second.matchOrder(received_order);

				if (received_order.getQuantity() > 0) {

					if (have_symbol == buy_side_symbols.end())
						have_symbol = buy_side_symbols.emplace(symbol, OrderBook(symbol)).first;

					(*have_symbol).second.addOrder(received_order);
				}
				else {

					std::cout << "BUY order with order id: " << received_order.getOrderId() << " traded" << std::endl;
					Order::id_to_order.erase(id);
				}
			}
		}
		else if (order_type == "SELL") {

			auto symbol_orderbook = buy_side_symbols.find(symbol);
			auto have_symbol = sell_side_symbols.find(symbol);

			if (symbol_orderbook == buy_side_symbols.end()) {

				if (have_symbol == sell_side_symbols.end())
					have_symbol = sell_side_symbols.emplace(symbol, OrderBook(symbol)).first;

				(*have_symbol).second.addOrder(received_order);
			}
			else {

				(*symbol_orderbook).second.matchOrder(received_order);

				if (received_order.getQuantity() > 0) {

					if (have_symbol == sell_side_symbols.end())
						have_symbol = sell_side_symbols.emplace(symbol, OrderBook(symbol)).first;

					(*have_symbol).second.addOrder(received_order);
				}
				else {

					std::cout << "SELL order with order id: " << received_order.getOrderId() << " traded" << std::endl;
					Order::id_to_order.erase(id);
				}
			}
		}
		else {	// modify order

			Order original_order = Order((*Order::id_to_order.find(id)).second);
			
			if (original_order.getSide() == BUY) {
						
				auto symbol_orderbook = buy_side_symbols.find(symbol);
				(*symbol_orderbook).second.modifyOrder(original_order, qty, price);
			}
			else {

				auto symbol_orderbook = sell_side_symbols.find(symbol);
				(*symbol_orderbook).second.modifyOrder(original_order, qty, price);
			}
		}
	}
}

int main() {
	
	takeOrders();
	return 0;
}
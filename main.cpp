#include <map>
#include "order.h"


std::vector<Symbol> sell_order_symbols;
std::map<std::string, int> sell_sym_at_idx;
std::vector<Symbol> buy_order_symbols;
std::map<std::string, int> buy_sym_at_idx;

// call this till Buy queue is not empty
void takeOrders() {
	
	while (true) {
		
		std::string order_type;
		float price;
		uint64_t qty;
		uint64_t id;
		std::string symbol;
		time_point<system_clock> curr_time = system_clock::now();

		std::cin >> order_type >> price >> qty >> id >> symbol;

		Order received_order = Order(price, qty, id, curr_time);

		auto sell_sym_idx = sell_sym_at_idx.find(symbol);
		auto buy_sym_idx = buy_sym_at_idx.find(symbol);

		if (order_type == "SELL") {

			if (buy_sym_idx != buy_sym_at_idx.end()) {

				buy_order_symbols[(*buy_sym_idx).second].executeSellOrder(received_order);
			}

			if (received_order.getQuantity() > 0) {

				if (sell_sym_idx == sell_sym_at_idx.end()) {

					sell_sym_at_idx[symbol] = sell_order_symbols.size();
					sell_order_symbols.push_back(Symbol(symbol));
					sell_order_symbols.back().addSellOrder(received_order);
				}
				else {

					sell_order_symbols[(*sell_sym_idx).second].addSellOrder(received_order);
				}
			}
			else {
				/* cout order id and order traded at time */
				std::cout << "Order traded with Order ID: " << id << std::endl;
			}
		}
		else {
			
			if (sell_sym_idx != sell_sym_at_idx.end()) {

				sell_order_symbols[(*sell_sym_idx).second].executeBuyOrder(received_order);
			}

			if (received_order.getQuantity() > 0) {
				
				if (buy_sym_idx == buy_sym_at_idx.end()) {

					buy_sym_at_idx[symbol] = buy_order_symbols.size();
					buy_order_symbols.push_back(Symbol(symbol));
					buy_order_symbols.back().addBuyOrder(received_order);
				}
				else {

					buy_order_symbols[(*buy_sym_idx).second].addBuyOrder(received_order);
				}
			}
			else {
				/* cout order id and order traded at time */
				std::cout << "Order traded with Order ID: " << id << std::endl;
			}
		}
	}
}

int main() {
	
	takeOrders();
	return 0;
}
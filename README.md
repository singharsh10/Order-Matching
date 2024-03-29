# Order Matching

A C++ based limit order matching engine which uses
price-time priority algorithm. Order matching is the 
process of accepting buy and sell orders for a security 
(or other fungible asset) and matching them to allow trading 
between parties who are otherwise unknown to each other.

An order matching engine is used on every stock exchange across the globe.
Each asset or symbol has its own **order book**.

## Contents of an order in the order book
* Price (bid for buy orders and ask for sell orders).
* Quantity/amount of the symbol being traded.
* Order Id (a unique id associated with each order)
* Time (when the order entered the system).

When an order enters the system it is checked against the existing 
order of the respective type (i.e against buy orders for sell order and
against sell orders for buy order). If the orders present within the 
order book can satisfy the current order then the order is traded. 

In case the order cannot be completely fullfilled then the order is added to the
buy/sell side order book of that symbol using price-time algorithm (in order
to match against future orders). 

Modifications of order present in the order book is also supported
which allows price and quantity to be modified.

Once the order is traded the order id is printed onto the console. 




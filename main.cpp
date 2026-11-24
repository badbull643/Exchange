#include <bits/stdc++.h>

/*

<<<<<<< HEAD
plan out the code here so
=======
so clients send orders over tcp a matching engine fills them by price time
priority,

and market data goes out over udp multicast,
every order is journaled to disk, so the exchange recovers after a crash
so this is  the testing file?

build the order book data structures,
matching logic,

order id lookup all single threaded ,and in processes,
so



so

for the order,
book

do
the price indexing by

what

so look a tthe matchine engine, so


one order book per symbol ,so





order book data structure,

so


matching logic, an order ID lookup all single threaded and in-process,

cancel/replace, that raises quna
reducing quantity keeps the priroty ? so

cancelling the order ?
the question is waht structure should i use ehre? so

8 symbols configured,

best price first,

in combing order, crosses the spread executes immediatly at the resting orders
price,

any remaining qunatity on a limit order rests on the book,

should
makret order ?so



the key features,
so



map is the first data structure i would consider,

so replace an order doesn nothing if you decrease the quantity
so this




flat array with price levels as an index, array would have too be massive tho?

so the resizing of the range,
so


looking at, the price

for a new price outside teh range resize, the vector which is constly but, once
book is established, not a problem so preallocate an initial range, so

so when price, goes outside the price range

so the tick matching mechanism so




>>>>>>> c1a97d53a5811ab2f68cdaaf955740a1e5ad1919



*/

class Symbol {
public:
private:
};

// the price level queue
struct PriceLevel {
  std::queue<long long> q;
  // other info
};

class OrderBook {
public:
  OrderBook() {}

private:
  // price levels
  std::vector<uint64_t> Price;
  uint64_t symbol_id;
  uint64_t
};

int main() {}

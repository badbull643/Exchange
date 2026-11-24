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

what does submit return? so



maybe submit returns the matches? so

so


iterate through the price level


tick = 1
smallest unit is 1,

so


100.85/tick

price, 10085 -> converted too what ?
100.85/tick



price, so

100.85 ->

so


for example 100.85 -> 10085

so


if we know the tick size then what?






a buy order against the ask book ,so

lowest buy at price <= limit buy


greatest sell at price >= limit ask


so we want the smallest value that exists so update a head for each book so

so smallest price first if the smallest price doesnt exist create a new teir,
the question is heap allocating orders? does it make sense ? store

[0,...,max] -> q1,q2,..,ql
so

is an optimisation skipping empty sections or maybe thats a compiler
optimisation? not sure? store

go through one queue


*/

struct Fills {
  Fills(uint64_t pay, uint64_t vol, uint64_t buy, uint64_t sell) {
    payment = pay;
    volume = vol;
    buyer_id = buy;
    seller_id = sell;
  }
  uint64_t payment;
  uint64_t volume;
  uint64_t buyer_id;
  uint64_t seller_id;
};

struct Order {
  // should be u8 antway so
  Order(bool type, uint64_t p, uint64_t v, uint64_t c) {
    isbid = type;
    price = p;
    volume = v;
    client_order_id = c;
  }
  bool isbid;
  uint64_t price;
  uint64_t volume;
  uint65_t client_order_id;
};

class Symbol {
public:
private:
};

// the price level queue so i need too store pointers too the orders so i can
// modify them so
//
struct PriceLevel {
  std::queue<Order> q;
  uint64_t sz() { return q.size(); };
};

class OrderBook {
public:
  OrderBook(uint64_t tick, uint64_t symbol_id, uint64_t init_max) {
    tick = tick;
    symbol_id = symbol_id;
    // initially just resize the price vector,
    ask_price.resize(init_max + 1);
    bid_price.resize(init_max + 1);
  }
  // first handle market/limit order so setting the price too 0 -> market
  // orde,Copyright
  std::vector<Fills> submit(Order o) {
    std::vector<Fills> f;
    if (o.isbid) {
      // add the bounds check here
      // scan from the back until, we get the min so if the min ask is > but
      // then we create a resting order at price, if the price is empty then we
      // iterate until we find non empty price tier? so
      while (o.volume > 0) {
        if (ask_price[ask_head].q.empty()) {
          // add any remaining volume as an order, so
          break;
        }
        // so while iterating through the queue,
        // so go too the next price tier maintain an array which stores the
        // current ask_heads? so delete the order pointer so what do i do with
        // th ask_price pointer so ? create a destructor for the class so for
        // large enough price ranges this O(n) scan gets worse though
        uint64_t cpy = ask_head;
        for (uint64_t h = ask_head; h < ask_price.size(); h++) {
          if (ask_head > o.price)
            break;
          while (!ask_price[h].q.empty()) {
            Order ask_order = ask_price[h].q.front();
            uint64_t diff = std::min(ask_order.volume, o.volume);
            ask_order.volume -= diff;
            o.volume -= diff;
            if (ask_order.volume == 0)
              ask_price[h].q.pop();
            if (o.volume == 0)
              break;
            // should return what needs too be transfered and too whom volume
            // transfered adn the price of the transfer,
            // fills edits the prices afterwards? is that fine or should it do
            // it in place? in place would require memory allocations first

            f.push_back(Fills(bid_id, ask_id, diff * ask_order->price, diff));
          }
          // update the askhead checks if the price tier is still emtpy else,
          if (ask_price[ask_head].empty())
            ask_head = h;
          // update askhead aswell
          if (o.volume == 0)
            break;
        }
        if (o.volume > 0) {
        }
      }
    }
  }

private:
  // price levels
  std::vector<PriceLevel> bid_price;
  std::vector<PriceLevel> ask_price;
  uint64_t symbol_id;
  uint64_t tick;
  // the heads point too non empty price tier, so
  uint64_t buy_head;
  uint64_t ask_head;
};

int main() {}

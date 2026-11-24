#include <bits/stdc++.h>
#include <cstdint>

/*

so the next part
is how do i cancel / edit orders so


when an order is inserted into a book,

keep a refrence too them so

since the orders are indexed by orderid (likely ill use some type of hexadecimal
too encode the orders but well see) so


for order ids use UUID so


so i need to write the unit tests for these,
keeping a refrence so a pointr, keep these contiguously some

refarray["order id"] -> refarray[]


map has terrible lookup times since orders will likely overflow then,
use a map ?
logn insertion is bad tho so

since its sorted, hashmap nogo since collisions,
so since queues cant be spliced and reput together maybe a better datastuctureso

lists structure but how should allocation occur so,

since i need erasure bascially so

l1 -> l2 -> l3 -> l4 ...
so


contiguously allocated space,
so when deletions occur, price levels can be deep

look at the expected price level prallocate a larger size for that price, level
,

then make sure lists are contiguous ? some


so preallocate a larger contiguous region,

since the cache locality of lists is terrible we wanna use a custom list struct


(l,r,val) the list object,
allocate rows, so


allocate all together

so preallocate the storage pool too use for the list prior so can reuse the
std::list so we dont have too recreate the list struct but i think that, i will
recreate the list so i get a bit more control over the list functions and
structure too make it easier too parse

so build the preallocated list struct since it should be the fastest so

get the node p

std::vector<Node> pool

so

from this pool just reuse the info hence should work


*/

struct B_list {};

struct Fills {
  Fills(int64_t pay, int64_t vol, int64_t buy, int64_t sell) {
    payment = pay;
    volume = vol;
    buyer_id = buy;
    seller_id = sell;
  }
  int64_t payment;
  int64_t volume;
  int64_t buyer_id;
  int64_t seller_id;
};

struct Order {
  // should be u8 antway so
  Order(bool type, int64_t p, int64_t v, int64_t c) {
    isbid = type;
    price = p;
    volume = v;
    client_order_id = c;
  }
  bool isbid;
  int64_t price;
  int64_t volume;
  int64_t client_order_id;
};

// the price level queue so i need too store pointers too the orders so i can
// modify them so
//
struct PriceLevel {
  std::queue<Order> q;
  int64_t sz() { return q.size(); };
};

class OrderBook {
public:
  OrderBook(int64_t tick, int64_t symbol_id, int64_t init_max) {
    this->tick = tick;
    this->symbol_id = symbol_id;
    bid_head = 0;
    ask_head = init_max;
    // initially just resize the price vector,
    ask_price.resize(init_max + 1);
    bid_price.resize(init_max + 1);
  }
  // first handle market/limit order so setting the price too 0 -> market
  // orde,Copyright
  std::vector<Fills> submit(Order o) {
    std::vector<Fills> f;

    if (o.price >= ask_price.size() || o.price < 0)
      return {};

    if (o.isbid) {
      // add the bounds check here
      // scan from the back until, we get the min so if the min ask is > but
      // then we create a resting order at price, if the price is empty then we
      int64_t limit = o.price == 0 ? ask_price.size() - 1 : o.price;
      int64_t cpy = ask_head;
      for (int64_t h = cpy; h < ask_price.size(); h++) {
        if (h > limit)
          break;
        while (o.volume > 0 && !ask_price[h].q.empty()) {
          Order &ask_order = ask_price[h].q.front();
          int64_t diff = std::min(ask_order.volume, o.volume);
          ask_order.volume -= diff;
          o.volume -= diff;

          f.push_back(Fills(diff * ask_order.price, diff, o.client_order_id,
                            ask_order.client_order_id));

          if (ask_order.volume == 0)
            ask_price[h].q.pop();
          // should return what needs too be transfered and too whom volume
          // transfered adn the price of the transfer
          // fills edits the prices afterwards? is that fine or should it do
          // it in place? in place would require memory allocations first
        }
        // update the askhead checks if the price tier is still emtpy else,
        if (ask_price[ask_head].q.empty())
          ask_head = h;
        // update askhead aswell
        if (o.volume == 0 && limit > 0)
          break;
      }
      if (o.volume > 0 && o.price > 0) {
        // insert the rest of the order so dont need too do the existance
        // check update the bid head if needed resting market volume? so if the
        // market is empty then the market order is reejct
        if (limit > bid_head)
          bid_head = limit;
        bid_price[limit].q.push(o);
      }
    } else {
      // the asks side of the match
      // handle the market order so that works if the price is 0
      int64_t cpy = bid_head;
      int64_t limit = o.price == 0 ? 0 : o.price;
      for (int64_t h = cpy; h >= 0; h--) {
        if (h < limit)
          break;
        while (o.volume > 0 && !bid_price[h].q.empty()) {
          Order &bid_order = bid_price[h].q.front();
          int64_t diff = std::min(bid_order.volume, o.volume);
          bid_order.volume -= diff;
          o.volume -= diff;

          f.push_back(Fills(diff * bid_order.price, diff,
                            bid_order.client_order_id, o.client_order_id));

          if (bid_order.volume == 0)
            bid_price[h].q.pop();
          // should return what needs too be transfered and too whom volume
          // transfered adn the price of the transfer
          // fills edits the prices afterwards? is that fine or should it do
          // it in place? in place would require memory allocations first
        }
        // update the askhead checks if the price tier is still emtpy else,
        if (bid_price[bid_head].q.empty())
          bid_head = h;
        // update askhead aswell
        if (o.volume == 0)
          break;
      }

      // add a check agains the market orders so it rejects it when the market
      // is empty so
      if (o.volume > 0 && o.price > 0) {
        // insert the rest of the order so dont need too do the existance
        // check update the bid head if needed ,
        if (limit < ask_head)
          ask_head = limit;
        ask_price[limit].q.push(o);
      }
    }

    return f;
  }

private:
  // price levels with the tick we can recover the rest of the array
  std::vector<PriceLevel> bid_price;
  std::vector<PriceLevel> ask_price;
  std::vector<int64_t> order_ref;
  int64_t symbol_id;
  int64_t tick;
  // the heads point too non empty price tier, so
  int64_t bid_head;
  int64_t ask_head;
};

int main() {}

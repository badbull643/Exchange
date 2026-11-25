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
will use a list for now but will implement this later sio



*/

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
// replace this with weakptr i forgot about weakptr
struct Ref {
  std::list<Order>::iterator r;
  bool validref;

  Ref() = default;

  Ref(std::list<Order>::iterator r, bool validref) {
    this->r = r;
    this->validref = validref;
  }
};

struct PriceLevel {
  std::list<Order> q;
  int64_t sz() { return q.size(); };
};

class OrderBook {
public:
  OrderBook(int64_t tick, int64_t symbol_id, int64_t init_max,
            int64_t order_buffer) {
    this->tick = tick;
    this->symbol_id = symbol_id;
    this->order_buffer = order_buffer;
    bid_head = 0;
    ask_head = init_max;
    // initially just resize the price vector,
    ask_price.resize(init_max + 1);
    bid_price.resize(init_max + 1);
    bid_order_ref.resize(order_buffer + 1);
    ask_order_ref.resize(order_buffer + 1);
    order_type.resize(order_buffer + 1);
  }
  // first handle market/limit order so setting the price too 0 -> market
  // orde
  //
  //
  //

  // replace the quueu with a list iterator so
  //
  std::vector<Fills> submit(Order o) {
    std::vector<Fills> f;

    if (o.price >= ask_price.size() || o.price < 0 || o.volume <= 0 ||
        o.client_order_id < 0 || o.client_order_id > order_buffer)
      return {};
    if (bid_order_ref[o.client_order_id].validref ||
        ask_order_ref[o.client_order_id].validref)
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
          Ref ask_order = Ref(ask_price[h].q.begin(), true);
          int64_t diff = std::min(ask_order.r->volume, o.volume);
          ask_order.r->volume -= diff;
          o.volume -= diff;

          f.push_back(Fills(diff * ask_order.r->price, diff, o.client_order_id,
                            ask_order.r->client_order_id));

          if (ask_order.r->volume == 0) {
            ask_order_ref[ask_order.r->client_order_id].validref = false;
            ask_price[h].q.pop_front();
          } // jere a deleteion is happening
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
        bid_price[limit].q.push_back(o);
        // not sure safe but seems ok does this need a guard ?
        bid_order_ref[o.client_order_id] =
            Ref(std::prev(bid_price[limit].q.end()), true);
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
          Ref bid_order = Ref(bid_price[h].q.begin(), true);
          int64_t diff = std::min(bid_order.r->volume, o.volume);
          bid_order.r->volume -= diff;
          o.volume -= diff;

          f.push_back(Fills(diff * bid_order.r->price, diff,
                            bid_order.r->client_order_id, o.client_order_id));

          if (bid_order.r->volume == 0) {
            bid_order_ref[bid_order.r->client_order_id].validref = false;
            bid_price[h].q.pop_front();
          }
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

        // so when an order is added too the book we want a refrence too the
        ask_price[limit].q.push_back(o);
        // not sure if this is a copy tho ?
        ask_order_ref[o.client_order_id] =
            Ref(std::prev(ask_price[limit].q.end()), true);
      }
    }
    if (o.volume > 0) {
      order_type[o.client_order_id].first = o.isbid;
      order_type[o.client_order_id].second = o.price;
    }
    return f;
  }

  // the main thing is that, so what is the main thing here its the package
  // loss? so

  // so replace should return false if we replace and raise loose price time
  // priority
  //
  //
  // add the ERROR FLAG TOO FILLS so we can just return it in the vector instead
  // of new scaffolding
  std::vector<Fills> replace(int64_t orig_client_order_id,
                             int64_t new_client_order_id, int64_t price,
                             int64_t qty) {
    // so use the replace ment order as the input order,
    //
    // create an actual error event later with the event log
    // create an error flag later
    if (orig_client_order_id > order_buffer || orig_client_order_id < 0 ||
        qty <= 0 || price <= 0 || price >= bid_price.size() ||
        new_client_order_id > order_buffer || new_client_order_id < 0)
      return {};

    if (new_client_order_id != orig_client_order_id &&
        (bid_order_ref[new_client_order_id].validref ||
         ask_order_ref[new_client_order_id].validref))
      return {};

    if (order_type[orig_client_order_id].first) {
      if (!bid_order_ref[orig_client_order_id].validref)
        return {};
      if (qty > bid_order_ref[orig_client_order_id].r->volume ||
          price != bid_order_ref[orig_client_order_id].r->price) {

        Order new_order = Order(bid_order_ref[orig_client_order_id].r->isbid,
                                price, qty, new_client_order_id);
        if (cancel(orig_client_order_id)) {
          return submit(new_order);
        } else {
          return {};
        }
      } else {
        bid_order_ref[orig_client_order_id].r->volume = qty;
      }
    } else {
      if (!ask_order_ref[orig_client_order_id].validref)
        return {};

      if (qty > ask_order_ref[orig_client_order_id].r->volume ||
          price != ask_order_ref[orig_client_order_id].r->price) {

        Order new_order = Order(ask_order_ref[orig_client_order_id].r->isbid,
                                price, qty, new_client_order_id);
        if (cancel(orig_client_order_id)) {
          return submit(new_order);
        } else {
          return {};
        }
      } else {
        ask_order_ref[orig_client_order_id].r->volume = qty;
      }
    }
    return {};
  }

  bool cancel(int64_t client_order_id) {
    if (client_order_id > order_buffer || client_order_id < 0)
      return false;
    if (order_type[client_order_id].first) {
      // already been erased so check wehter that order still exists within the
      // list ? so we dont
      if (bid_order_ref[client_order_id].validref) {
        bid_price[order_type[client_order_id].second].q.erase(
            bid_order_ref[client_order_id].r);
        bid_order_ref[client_order_id].validref = false;
        return true;
      }
    } else {
      // ask orders
      if (ask_order_ref[client_order_id].validref) {
        ask_price[order_type[client_order_id].second].q.erase(
            ask_order_ref[client_order_id].r);
        ask_order_ref[client_order_id].validref = false;
        return true;
      }
    }
    return false;
  }
  // keep a trackr for when the objects have been erased so
private:
  // price levels with the tick we can recover the rest of the array
  std::vector<PriceLevel> bid_price;
  std::vector<PriceLevel> ask_price;
  // so this give sthe vector id a location so could preallocate full of
  // nullptrs but not sure
  std::vector<Ref> ask_order_ref;
  std::vector<Ref> bid_order_ref;
  std::vector<std::pair<bool, int64_t>> order_type;
  int64_t symbol_id;
  int64_t tick;
  // the heads point too non empty price tier, so
  int64_t bid_head;
  int64_t ask_head;
  int64_t order_buffer;
};

int main() {
  // so place a couple of orders so
  //
}

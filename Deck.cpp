// Created by CBunt on 05 May 2020.
//

#include "Deck.h"

Deck::Deck() {
  // for(auto k : cards)
    for(int f = 1; f < 14; ++f)
      for(int r = 1; r < 5; ++r)
	cards.emplace_front(r, f);
    Shuffle();
}


void Deck::Shuffle() {
  //Implementation of shuffle can be found at:
  //http://www.cplusplus.com/reference/algorithm/shuffle/
  unsigned seed = std::crono::system_clock::now().time_since_epoch().count();

  shuffle(cards.begin(), cards.end(), std::default_random_engine());
}

*card Deck::Deal(int quant) {
  array<card, quant> ret 
}

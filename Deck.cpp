// Created by CBunt on 05 May 2020.
//

#include "Deck.h"

#include <random>

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
  rng = RandomEngine<int>(1,52);
  Build();
  Shuffle();
}

Deck::~Deck() = default;

*card Deck::Deal(int quant) {
  array<card, quant> ret
void Deck::Build() {
  for(int s = 1; s < 5; ++s)
    for(int r = 1; r < 14; ++r)
      Cards.emplace_back(r,s);
}

void Deck::Shuffle() {
  std::shuffle(Cards.begin(), Cards.end(), std::mt19937(std::random_device()()));
}
// Created by CBunt on 05 May 2020.
//

#include "Deck.h"

Deck::~Deck() = default;

Deck::Deck() {
  //rng = new RandomEngine<int>(0,51);
  Build();
  Shuffle();
}

void Deck::Build() {
  for(int s = 1; s < 5; ++s)
    for(int r = 2; r < 15; ++r)
      Cards.emplace_back(r,s);
}

void Deck::Shuffle() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(Cards.begin(), Cards.end(),
               std::default_random_engine(seed));
}

cards Deck::Deal(int quant) {
  cards ret;
  for(int k = 0; k < quant; ++k) {
    ret.push_back(Cards.back());
    Cards.pop_back();
  }
  return ret;
}

card Deck::Burn() {
  card ret = Cards.back();
  Cards.pop_back();
  return ret;
}


cards operator+(const cards lhs, const cards rhs) {
  cards ret = lhs;
  for(auto k : rhs)
    ret.emplace_back(k);
  return cards();
}
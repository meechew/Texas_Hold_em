// Created by CBunt on 05 May 2020.
//

#include "Deck.hpp"

Deck::~Deck() = default;

Deck::Deck() {
  rng = new RandomEngine(0,51);
  build();
  shuffle();
}

void Deck::build() {
  for(int s = 1; s < 5; ++s)
    for(int r = 2; r < 15; ++r)
      cards_.emplace_back(r,s);
}

void Deck::shuffle() {
  boost::range::random_shuffle(cards_,*rng);
}

Cards Deck::deal(const int count) {
  Cards ret;
  for(int k = 0; k < count; ++k) {
    ret.push_back(cards_.back());
    cards_.pop_back();
  }
  return ret;
}

Card Deck::burn() {
  Card ret = cards_.back();
  cards_.pop_back();
  return ret;
}

Cards operator+(const Cards& lhs, const Cards& rhs) {
  Cards ret = lhs;
  for(auto k : rhs)
    ret.emplace_back(k);
  return ret;
}

Cards operator+(const Cards& lhs, const Hand& rhs) {
  Cards ret = lhs;
  ret.emplace_back(rhs.first);
  ret.emplace_back(rhs.second);
  return ret;
}

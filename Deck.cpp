// Created by CBunt on 05 May 2020.
//

#include "Deck.h"

#include <random>

Deck::Deck() {
  rng = RandomEngine<int>(1,52);
  Build();
  Shuffle();
}

Deck::~Deck() = default;

void Deck::Build() {
  for(int s = 1; s < 5; ++s)
    for(int r = 1; r < 14; ++r)
      Cards.emplace_back(r,s);
}

void Deck::Shuffle() {
  std::shuffle(Cards.begin(), Cards.end(), std::mt19937(std::random_device()()));
}
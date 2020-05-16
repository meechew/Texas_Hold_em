// Created by CBunt on 16 May 2020.
//

#include "Deck.h"

int main() {
  Deck T_Deck;
  cards T_Hand = T_Deck.Deal(5);
  for( auto k : T_Hand) {
    std::cout << "-->"<< Rank[k.rank] << SuitChar[k.suit] << std::endl;
  }
}
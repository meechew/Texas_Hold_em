// Created by CBunt on 16 May 2020.
//

#include "Deck.hpp"
#include "Player.hpp"
#include "FaceValues.hpp"

int main() {
  Deck T_Deck;
  cards T_Hand = T_Deck.Deal(5);
  for( auto k : T_Hand) {
    std::cout << "-->"<< Rank[k.rank] << SuitChar[k.suit] << std::endl;
  }
  Player Player1("Player1");
  Player1.NewHand(T_Deck.Deal(2));
  T_Hand = Player1.Call();
  std::cout << "Player1 Name -->" << Player1.Who() << std::endl <<
            "Player1 Hand -->" << Rank[T_Hand.front().rank] <<
            SuitChar[T_Hand.front().suit] << ',' << Rank[T_Hand.back().rank] <<
            SuitChar[T_Hand.back().suit];

  Player1.Fold();
  std::cout << "Player1 Fold -->" << Rank[T_Hand.front().rank] << SuitChar[T_Hand.front().suit] <<
            ',' << Rank[T_Hand.back().rank] << SuitChar[T_Hand.back().suit];


}

// Created by CBunt on 16 May 2020.
//

#include "Deck.hpp"
#include "Player.hpp"
#include "FaceValues.hpp"
#include "Package.hpp"

int main() {
  Deck T_Deck;
  hand T_Hand;
  cards T_Common;
  T_Hand.first = T_Deck.Deal(1).front();
  T_Hand.second = T_Deck.Deal(1).front();

  T_Common = T_Deck.Deal(5);

  std::cout << "common cards";
  for( auto k : T_Common) {
    std::cout << "-->"<< Rank[k.rank] << SuitChar[k.suit] << std::endl;
  }
  Player Player1("Player1");
  Player1.NewHand(T_Deck.Deal(2));
  T_Hand = Player1.Call();
  std::cout << "Player1 Name -->" << Player1.Who() << std::endl <<
            "Player1 Hand -->" << Rank[T_Hand.first.rank] <<
            SuitChar[T_Hand.first.suit] << ',' << Rank[T_Hand.second.rank] <<
            SuitChar[T_Hand.second.suit];

  Player1.Fold();
  std::cout << "Player1 Fold -->" << Rank[T_Hand.first.rank] << SuitChar[T_Hand.first.suit] <<
            ',' << Rank[T_Hand.second.rank] << SuitChar[T_Hand.second.suit];

  ServerPackage Pack;
  Pack.HeartBeat = false;
  Pack.WinnerNotice = false;
  Pack.Name = std::move(Player1.Who());
  Pack.Winner = "Winner!!";
  Pack.Hand = Player1.Call();
  for(int k = 0; k < 5; ++k) {

  }

  std::cout << "\nPackaged: \n" << Pack;
}

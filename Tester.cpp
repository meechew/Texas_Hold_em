// Created by CBunt on 16 May 2020.
//

#include "Deck.hpp"
#include "Player.hpp"
#include "FaceValues.hpp"
#include "Package.hpp"
#include "NetworkController.hpp"

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
            SuitChar[T_Hand.second.suit] << std::endl;

  Player1.Fold();
  T_Hand = Player1.Call();
  std::cout << "Player1 Fold -->" << Rank[T_Hand.first.rank] << SuitChar[T_Hand.first.suit] <<
            ',' << Rank[T_Hand.second.rank] << SuitChar[T_Hand.second.suit];

  Player1.NewHand(T_Deck.Deal(2));
  ServerPackage Pack(false,false, false, std::move(Player1.Who()),"Winner!!",Player1.Call(),T_Common);
  Pack.HeartBeat = false;
  Pack.WinnerNotice = false;
  Pack.Name = std::move(Player1.Who());
  Pack.Winner = "Winner!!";
  Pack.Hand = Player1.Call();
  for(int k = 0; k < 5; ++k) {
    Pack.Tbl[k] = T_Common[k];
  }

  std::cout << "\nPackaged: \n" << Pack << std::endl;



  Table T_Table;
  T_Table.NewPlayer(std::move(Player1.Who()));
  T_Table.Deal();
  T_Table.Flop();
  T_Table.River();
  T_Table.Turn();
  std::cout << "\nPackaged: \n" << *T_Table.Package(0, false, false, false) << '\n';
}

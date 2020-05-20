// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_PLAYER_H
#define TEXAS_HOLD_EM_PLAYER_H
#include <string>
#include <utility>
#include "Deck.h"

class Player {
private:
  std::string Name = nullptr;
  pair hand = pair(card(0,0),card(0,0));
public:
  Player() = default;
  Player(std::string n): Name(std::move(n)) {}
  void NewHand(cards Dealt);
  void Draw(cards Dealt, bool First, bool Second);
  void AddPlayer(std::string n);
  void Fold();
  cards Call();
};


#endif //TEXAS_HOLD_EM_PLAYER_H

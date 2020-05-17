// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_PLAYER_H
#define TEXAS_HOLD_EM_PLAYER_H
#include <string>
#include "Deck.h"

class Player {
private:
  std::string Name = nullptr;
  std::pair<card,card> hand = std::make_pair<card,card>(card(0,0),card(0,0));
public:
  Player() = default;
  void NewHand(cards Dealt);
  void Draw(cards Dealt, bool First, bool Second);
};


#endif //TEXAS_HOLD_EM_PLAYER_H

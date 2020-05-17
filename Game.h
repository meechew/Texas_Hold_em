// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_GAME_H
#define TEXAS_HOLD_EM_GAME_H

#include "Deck.h"

class Game {
private:
  Deck Dealer;
public:
  cards Flop();
  card River();
  card Turn();
};


#endif //TEXAS_HOLD_EM_GAME_H

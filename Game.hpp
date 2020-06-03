// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_GAME_HPP
#define TEXAS_HOLD_EM_GAME_HPP

#include "Deck.hpp"

class Game {
private:
  Deck Dealer;
public:
  cards Deal(int quant) {return Dealer.Deal(quant);}
  cards Flop();
  card River();
  card Turn();
};


#endif //TEXAS_HOLD_EM_GAME_HPP

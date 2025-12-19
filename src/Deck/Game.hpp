// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_GAME_HPP
#define TEXAS_HOLD_EM_GAME_HPP

#include "Deck.hpp"

class Game {
private:
  Deck dealer_;
public:
  Cards deal(const int count) {return dealer_.deal(count);}
  Cards flop();
  Card  river();
  Card  turn();
};


#endif //TEXAS_HOLD_EM_GAME_HPP

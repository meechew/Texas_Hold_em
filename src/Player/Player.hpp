// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_PLAYER_HPP
#define TEXAS_HOLD_EM_PLAYER_HPP
#include <utility>
#include <boost/container/string.hpp>
#include "../Deck/Deck.hpp"



class Player {
private:
  string name_{""};
  Hand   hand_;
public:
  Player() = default;
  explicit Player(string n):
      name_(std::move(n)) {}
  void   new_hand(Cards c);
  void   add_player(string n);
  void   fold();
  string who();
  Hand   call();
};


#endif //TEXAS_HOLD_EM_PLAYER_HPP

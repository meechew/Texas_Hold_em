// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_PLAYER_HPP
#define TEXAS_HOLD_EM_PLAYER_HPP
#include <utility>
#include <boost/container/string.hpp>
#include "Deck.hpp"



class Player {
private:
  string Name{""};
  hand Hand;
public:
  Player() = default;
  explicit Player(string n):
      Name(std::move(n)) {}
  void NewHand(cards Dealt);
  void Draw(cards Dealt, bool First, bool Second);
  void AddPlayer(string n);
  void Fold();
  string Who();
  hand Call();
};


#endif //TEXAS_HOLD_EM_PLAYER_HPP

// Created by CBunt on 16 May 2020.
//

#include "Player.hpp"

void Player::new_hand(Cards c) {
  hand_.first = c.front();
  hand_.second = c.back();
}

void Player::add_player(string n) {
  name_ = std::move(n);
}

Hand Player::call() {
  return hand_;
}

void Player::fold() {
  hand_ = std::make_pair<Card,Card>(Card(0, 0), Card(0, 0));
}

string Player::who() {
  return name_;
}

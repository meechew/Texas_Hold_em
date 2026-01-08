// Created by CBunt on 16 May 2020.
//

#include "Player.hpp"

void Player::new_hand(Cards c) {
  hand_.first = c.front();
  hand_.second = c.back();
}

void Player::add_player(string n) {
  name_ = boost::move(n);
}

Hand Player::call() {
  return hand_;
}

void Player::fold() {
  hand_ = Hand();
}

string Player::who() {
  return name_;
}

uuid Player::get_uuid() const
{
  return uuid_;
}

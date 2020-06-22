// Created by CBunt on 16 May 2020.
//

#include "TexasServer.hpp"

void Player::NewHand(cards Dealt) {
  Hand.first = Dealt.front();
  Hand.second = Dealt.back();
}

void Player::AddPlayer(string n) {
  Name = std::move(n);
}

hand Player::Call() {
  return Hand;
}

void Player::Fold() {
  Hand = std::make_pair<card,card>(card(0, 0), card(0, 0));
}

string Player::Who() {
  return Name;
}

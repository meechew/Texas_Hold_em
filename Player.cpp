// Created by CBunt on 16 May 2020.
//

#include "Player.hpp"

void Player::NewHand(cards Dealt) {
  Hand.first = Dealt.front();
  Hand.second = Dealt.back();
}

// For switching cards drawn. Because a single card is the front
// or the back of the vector this works for both 1 or 2 cards.
// but we do not switch cards in texas hold em
void Player::Draw(cards Dealt, bool First, bool Second) {
  if (First)
    Hand.first = Dealt.front();
  if (Second)
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

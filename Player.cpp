// Created by CBunt on 16 May 2020.
//

#include "Player.h"

void Player::NewHand(cards Dealt) {
  hand.first = Dealt.front();
  hand.second = Dealt.back();
}

// For switching cards drawn. Because a single card is the front
// or the back of the vector this works for both 1 or 2 cards.
// but we do not switch cards in texas hold em
void Player::Draw(cards Dealt, bool First, bool Second) {
  if (First)
    hand.first = Dealt.front();
  if (Second)
    hand.second = Dealt.back();
}



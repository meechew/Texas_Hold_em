// Created by CBunt on 16 May 2020.
//

#include "Game.hpp"


cards Game::Flop() {
  Dealer.Burn();
  return Dealer.Deal(3);
}

card Game::River() {
  Dealer.Burn();
  return Dealer.Deal(1).back();
}

card Game::Turn() {return River();}
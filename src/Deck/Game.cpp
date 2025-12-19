// Created by CBunt on 16 May 2020.
//

#include "Game.hpp"


Cards Game::flop() {
  dealer_.burn();
  return dealer_.deal(3);
}

Card Game::river() {
  dealer_.burn();
  return dealer_.deal(1).back();
}

Card Game::turn() {return river();}
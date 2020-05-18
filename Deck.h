// Created by CBunt on 05 May 2020.
//

#ifndef TEXAS_HOLD_EM_DECK_H
#define TEXAS_HOLD_EM_DECK_H
#include <iostream>
#include <algorithm>
#include <list>
#include <random>
#include <chrono>
#include <map>
//#include "RandomEngine.h"
#define CLUB  2660
#define DIAM  2665
#define HEART 2663
#define SPADE 2666


struct card {
  int rank;
  int suit;
  card(int r,int s): rank(r), suit(s) {}
};

typedef std::vector<card> cards;
cards operator+(const cards lhs, const cards rhs);

class Deck {
private:
  cards Cards;
  //RandomEngine<int> *rng;
  void Build();
public:
  Deck();
  ~Deck();
  void Shuffle();
  cards Deal(int quant);
  card Burn();
};

#endif //TEXAS_HOLD_EM_DECK_H

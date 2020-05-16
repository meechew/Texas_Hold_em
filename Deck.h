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
#define CLUB  9827
#define DIAM  9830
#define HEART 9829
#define SPADE 9824


struct card {
  int rank;
  int suit;
  card(int r,int s): rank(r), suit(s) {}
};

typedef std::vector<card> cards;

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

};

#endif //TEXAS_HOLD_EM_DECK_H

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
  card(): rank(0), suit(0) {}
  card(int r,int s): rank(r), suit(s) {}
  inline bool operator==(const card &test) const {
    return test.rank == rank and test.suit == suit;
  }
  inline bool operator<(card & c) const {
    return rank < c.rank;
  }
};

typedef std::vector<card> cards;
typedef std::pair<card,card> pair;
cards operator+(cards lhs, cards rhs);

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

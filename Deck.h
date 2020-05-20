// Created by CBunt on 05 May 2020.
//

// compile with -lstdc++

#ifndef TEXAS_HOLD_EM_DECK_H
#define TEXAS_HOLD_EM_DECK_H
#include <iostream>
#include <chrono>
#include <list>
#include <map>
#include <boost/algorithm/algorithm.hpp>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <boost/container/map.hpp>
#include "RandomEngine.h"
#define CLUB  2660
#define DIAM  2665
#define HEART 2663
#define SPADE 2666

using namespace boost::container;

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

typedef vector<card> cards;
typedef std::pair<card,card> pair;
cards operator+(cards lhs, cards rhs);

class Deck {
private:
  cards Cards;
  RandomEngine *rng;
  void Build();
public:
  Deck();
  ~Deck();
  void Shuffle();
  cards Deal(int quant);
  card Burn();
};

#endif //TEXAS_HOLD_EM_DECK_H

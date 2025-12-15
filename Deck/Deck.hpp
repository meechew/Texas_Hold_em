// Created by CBunt on 05 May 2020.
//

// compile with -lstdc++

#ifndef TEXAS_HOLD_EM_DECK_HPP
#define TEXAS_HOLD_EM_DECK_HPP
#include <iostream>
#include <chrono>
#include <boost/range/algorithm/sort.hpp>
#include <boost/container/container_fwd.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <boost/container/map.hpp>
#include <boost/container/vector.hpp>
#include "../GameAssets/RandomEngine.hpp"
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
typedef std::pair<card, card> hand;
cards operator+(const cards& lhs, const cards& rhs);
cards operator+(const cards& lhs, const hand& rhs);

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

#endif //TEXAS_HOLD_EM_DECK_HPP

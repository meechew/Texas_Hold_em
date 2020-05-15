// Created by CBunt on 05 May 2020.
//

#ifndef TEXAS_HOLD_EM_DECK_H
#define TEXAS_HOLD_EM_DECK_H
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include "RandomEngine.h"
#define CLUB  9827
#define DIAM  9830
#define HEART 9829
#define SPADE 9824

char *Rank[] = {
    [1] "A",
    [2] "2",
    [3] "3",
    [4] "4",
    [5] "5",
    [6] "6",
    [7] "7",
    [8] "8",
    [9] "9",
    [10] "T",
    [11] "J",
    [12] "Q",
    [13] "K"
};

char *SuitSym[] = {
    [1] = "\u2660",
    [2] = "\u2665",
    [3] = "\u2663",
    [4] = "\u2666"
};

char *SuitChar[] = {
    [1] = "S",
    [2] = "H",
    [3] = "C",
    [4] = "D"
};

struct card {
  int rank;
  int suit;
  card(int r,int s): rank(r), suit(s) {}
};

class Rand {

};

typedef std::vector<card> cards;

class Deck {
  cards Cards;
  RandomEngine<int> rng;
  void Build();
  list<card> cards;
  //array<card, 52> cards;
    //  card cards[52];
public:
  Deck();
  ~Deck();
  void Shuffle();

};

#endif //TEXAS_HOLD_EM_DECK_H

// Created by CBunt on 05 May 2020.
//

#ifndef TEXAS_HOLD_EM_DECK_H
#define TEXAS_HOLD_EM_DECK_H
#include <iostream>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>
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

char *Face[] = {
    [1] "\u9827",
    [2] "\u9830",
    [3] "\u9829",
    [4] "\u9824"
};
struct card {
  int rank;
  int face;
};

class Deck {
  card cards[52];
public:
  Deck();
  ~Deck();
  void Shuffle();

};

#endif //TEXAS_HOLD_EM_DECK_H

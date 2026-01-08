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
#include <boost/container/vector.hpp>
#include "../GameUtilities/RandomEngine.hpp"
#define CLUB  2660
#define DIAM  2665
#define HEART 2663
#define SPADE 2666

using namespace boost::container;

struct Card
{
    int rank;
    int suit;

    Card() : rank(0), suit(0) {}
    Card(int r, int s) : rank(r), suit(s) {}

    inline bool operator==(const Card& test) const
    {
        return test.rank == rank and test.suit == suit;
    }

    inline bool operator<(Card& c) const
    {
        return rank < c.rank;
    }

    Card& operator=(const Card& c);
};

typedef vector<Card> Cards;

struct Hand
{
    Card first;
    Card second;
    Hand() : first(), second() {}

    explicit Hand(Cards c)
    {
        if (c.size() != 2)
        {
            throw_length_error("Hand must be 2 Cards");
        }

        first = c.front();
        second = c.back();
    }
};

Cards operator+(const Cards& lhs, const Cards& rhs);
Cards operator+(const Cards& lhs, const Hand& rhs);

class Deck
{
private:
    Cards cards_;
    RandomEngine* rng;
    void build();

public:
    Deck();
    ~Deck();
    void shuffle();
    Cards deal(int count);
    Hand deal_hand();
    Card burn();
};

#endif //TEXAS_HOLD_EM_DECK_HPP

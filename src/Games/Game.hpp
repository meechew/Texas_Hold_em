// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_GAME_HPP
#define TEXAS_HOLD_EM_GAME_HPP

#include "../Deck/Deck.hpp"

struct ScoreBoard
{
    Card high_card_ = Card(0, 0);
    bool straight_flush_ = false;
    bool four_kind_ = false;
    bool full_house_ = false;
    bool flush_ = false;
    bool straight_ = false;
    bool three_kind_ = false;
    bool two_pair_ = false;
    bool one_pair_ = false;
    ScoreBoard() = default;

    ScoreBoard(bool StraightFlush, bool FourKind, bool FullHouse, bool Flush, bool Straight,
               bool ThreeKind, bool TwoPair, bool OnePair, const Card& HighCard) :
        high_card_(HighCard), straight_flush_(StraightFlush),
        four_kind_(FourKind), full_house_(FullHouse), flush_(Flush), straight_(Straight),
        three_kind_(ThreeKind), two_pair_(TwoPair), one_pair_(OnePair)
    {
    }
};

// Abstract Class
class Game {
protected:
    struct hand_;
    Deck dealer_;
    int  step_counter = 0;
    virtual void end_game() =0;
public:
    Cards deal(const int cnt) { return dealer_.deal(cnt); }
    virtual ~Game() = default;
    virtual void next_step() = 0;
};


#endif //TEXAS_HOLD_EM_GAME_HPP

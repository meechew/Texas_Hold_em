// Created by meechew on 12/27/25.
//

#ifndef TEXAS_HOLD_EM_TEXASHOLDEM_HPP
#define TEXAS_HOLD_EM_TEXASHOLDEM_HPP
#include "Game.hpp"


class TexasHoldEm final : public Game
{

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
    } hand_;

public:
    using Game::deal;
    Cards flop()  override;
    Card  river() override;
    Card  turn()  override;

private:
    void  next_step() override;
    void  deal();
    void  end_game() override;
};


#endif //TEXAS_HOLD_EM_TEXASHOLDEM_HPP
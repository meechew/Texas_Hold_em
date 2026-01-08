// Created by meechew on 12/27/25.
//

#include "TexasHoldEm.hpp"


void TexasHoldEm::next_step()
{
    switch (step_counter)
    {
        case 0:
            deal();
            break;
        case 1:
            flop();
            break;
        case 2:
            river();
            break;
        case 3:
            turn();
            break;

        default: break;
    }
    step_counter++;
}

void TexasHoldEm::deal()
{
    // ToDo: properly deal cards 1 by 1.
}

Cards TexasHoldEm::flop() {
    dealer_.burn();
    return dealer_.deal(3);
}

Card TexasHoldEm::river() {
    dealer_.burn();
    return dealer_.deal(1).back();
}

Card TexasHoldEm::turn() { return river(); }

void TexasHoldEm::end_game()
{
    // ToDo: trigger scoring and roud decision;
}

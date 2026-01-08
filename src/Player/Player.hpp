// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_PLAYER_HPP
#define TEXAS_HOLD_EM_PLAYER_HPP
#include <utility>
#include <boost/container/string.hpp>
#include <boost/uuid/uuid.hpp>

#include "../Deck/Deck.hpp"

using namespace boost::uuids;

class Player
{
private:
    string name_{""};
    Hand hand_;
    uuid uuid_;
public:
    Player() = default;

    explicit Player(string n, uuid u) :
        name_(boost::move(n)), uuid_(u) {}

    void new_hand(Cards c);
    void add_player(string n);
    void fold();
    string who();
    uuid get_uuid() const;
    Hand call();
};


#endif //TEXAS_HOLD_EM_PLAYER_HPP

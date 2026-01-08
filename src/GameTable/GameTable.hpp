// Created by meechew on 12/27/25.
//

#ifndef TEXAS_HOLD_EM_GAMETABLE_HPP
#define TEXAS_HOLD_EM_GAMETABLE_HPP
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include "../Games/Game.hpp"
#include "../Player/Player.hpp"
#include "../NetworkAssets/Message.hpp"
#include "../NetworkAssets/Package.hpp"
#include "../NetworkAssets/NetworkController.hpp"

typedef boost::array<Player, 5> Players;

class GameTable
{
public:
    Deck game_deck_;
    Cards common_cards_;

    void add_thread(boost::shared_ptr<boost::thread> t)
    {
        server_thread_ = std::move(t);
    }

    void incoming_player(SeatPtr s, ClientPackage p);
    void player_leave(SeatPtr s);
    void process_package(ClientPackage p);
    void game_start();
    void step();

private:
    int stage_ = 0;
    Game* game_table_ = nullptr;
    PlayerFinals final_hand_;
    SocketSeatLink seated_players_;
    boost::shared_ptr<boost::thread> server_thread_;

    SeatPtr find_socket(Player p)
    {
            for (auto ssp: seated_players_)
            {
                if (ssp.get_right()->get_uuid() == p.get_uuid())
                    return ssp.get_left();
            }
        return nullptr;
    }
    void add_play(SeatPtr s, PlayerPtr p)
    {
        seated_players_.insert(SocketSeatPair(s, p));
    }

    static ScoreBoard tabulate(const Cards&);
    void deal();
    Cards flop();
    Card turn();
    Card river();
    Player* check_for_winner();
    ServerPackage* make_package(PlayerPtr p, bool hb, bool wn, bool sp) const;
};


#endif //TEXAS_HOLD_EM_GAMETABLE_HPP

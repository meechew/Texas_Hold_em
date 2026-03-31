// Created by meechew on 12/27/25.
//

#ifndef TEXAS_HOLD_EM_GAMETABLE_HPP
#define TEXAS_HOLD_EM_GAMETABLE_HPP

#include <deque>
#include <boost/array.hpp>
#include <boost/bimap.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "../Games/Game.hpp"
#include "../Games/TexasHoldEm.hpp"
#include "../Player/Player.hpp"
#include "../NetworkAssets/Message.hpp"
#include "../NetworkAssets/Package.hpp"
#include "../NetworkAssets/NetworkController.hpp"

using namespace boost::uuids;

// SeatPtr is a network session handle — used to send messages back to a client
typedef NetworkController::ptr SeatPtr;
typedef boost::shared_ptr<Player> PlayerPtr;

// Bimap: uuid (left) <-> session pointer (right)
typedef boost::bimap<uuid, SeatPtr> SeatMap;
typedef SeatMap::value_type SeatPair;

struct Finals
{
    PlayerPtr player_;
    Cards final_hand_;
    ScoreBoard final_score_;
};

class GameTable
{
public:
    Cards common_cards_;

    // Called by ServerController when a message arrives on a session
    void incoming_update(const Message& msg, SeatPtr session);

    // Called when a player's first message is received (registers them)
    void incoming_player(SeatPtr s, const ClientPackage& p);

    // Called when a session disconnects
    void player_leave(SeatPtr s);

    // Process a queued message — uses msg.uuid() to identify the sender
    void process_package(const Message& msg);

    void game_start();
    void step();

    [[noreturn]] void start_heart_beat();

    // Query helpers (public for testing)
    bool has_player(const std::string& name) const;
    std::size_t player_count() const { return seated_players_.size(); }

private:
    int stage_ = 0;
    TexasHoldEm game_;
    boost::array<Player, 5> host_players_;
    SeatMap seated_players_;
    boost::array<Finals, 5> final_hand_;
    std::deque<Message> incoming_queue_;

    int find_open_seat() const;
    Player* find_player_by_uuid(const uuid& u);
    void signal(const uuid& u, const Message& msg);

    static ScoreBoard tabulate(const Cards& hand);
    void deal();
    void flop();
    void turn_card();
    void river_card();
    Player* check_for_winner();
    ServerPackage make_package(const Player& p, bool hb, bool wn, bool sp) const;
};

#endif //TEXAS_HOLD_EM_GAMETABLE_HPP
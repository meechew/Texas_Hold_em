// Created by meechew on 12/27/25.
//

#include <iostream>
#include <cstring>
#include <sstream>
#include <boost/interprocess/streams/bufferstream.hpp>
#include "GameTable.hpp"

// ─── Public query helpers ─────────────────────────────────────────────────────

bool GameTable::has_player(const std::string& name) const
{
    for (const auto& p : host_players_)
        if (std::string(p.who().c_str()) == name)
            return true;
    return false;
}

// ─── Helpers ──────────────────────────────────────────────────────────────────

int GameTable::find_open_seat() const
{
    for (int k = 0; k < 5; ++k)
        if (host_players_[k].who().empty())
            return k;
    return -1;
}

Player* GameTable::find_player_by_uuid(const uuid& u)
{
    for (auto& p : host_players_)
        if (p.get_uuid() == u)
            return &p;
    return nullptr;
}

// Write a message to a specific player's session
void GameTable::signal(const uuid& u, const Message& msg)
{
    auto it = seated_players_.left.find(u);
    if (it != seated_players_.left.end())
        it->second->write_message(msg);
}

// ─── Player management ────────────────────────────────────────────────────────

void GameTable::incoming_player(SeatPtr s, const ClientPackage& p)
{
    // Already seated — ignore duplicate joins
    if (seated_players_.left.count(p.uuid_))
        return;

    int seat = find_open_seat();
    if (seat < 0) return;   // Table full

    host_players_[seat] = Player(boost::container::string(p.name_.c_str()), p.uuid_);
    seated_players_.insert(SeatPair(p.uuid_, s));

    std::cout << "--Player joined: " << p.name_ << "--\n";
}

void GameTable::player_leave(SeatPtr s)
{
    auto it = seated_players_.right.find(s);
    if (it == seated_players_.right.end()) return;

    uuid u = it->second;
    Player* p = find_player_by_uuid(u);
    if (p) p->fold();
    seated_players_.right.erase(it);

    std::cout << "--Player left--\n";
}

// ─── Message routing ──────────────────────────────────────────────────────────

// Entry point called by ServerController for every received message.
// Uses Message::uuid_ (header bytes 4-19) to identify the sender.
void GameTable::incoming_update(const Message& msg, SeatPtr session)
{
    uuid u;
    std::memcpy(u.data, msg.uuid(), 16);

    if (!seated_players_.left.count(u))
    {
        // First message from this UUID — decode package and register player
        ClientPackage pkg;
        boost::interprocess::bufferstream bs(const_cast<char*>(msg.body()),
                                             msg.get_body_length());
        bs >> pkg;
        incoming_player(session, pkg);
    }
    else
    {
        incoming_queue_.push_back(msg);
    }
}

void GameTable::process_package(const Message& msg)
{
    uuid u;
    std::memcpy(u.data, msg.uuid(), 16);

    Player* p = find_player_by_uuid(u);
    if (!p) return;

    ClientPackage pkg;
    boost::interprocess::bufferstream bs(const_cast<char*>(msg.body()),
                                         msg.get_body_length());
    bs >> pkg;

    if (pkg.heart_beat_)
        return;

    if (pkg.next_step_)
    {
        step();
        return;
    }

    if (pkg.leave_)
    {
        auto it = seated_players_.left.find(u);
        if (it != seated_players_.left.end())
            player_leave(it->second);
    }
}

// ─── Game flow ────────────────────────────────────────────────────────────────

void GameTable::game_start()
{
    game_  = TexasHoldEm();
    stage_ = 0;
    common_cards_.clear();
    for (auto& f : final_hand_) f = Finals{};
    std::cout << "--New game started--\n";
}

void GameTable::step()
{
    switch (stage_)
    {
    case 0: deal();             std::cout << "\tDeal\n";   break;
    case 1: flop();             std::cout << "\tFlop\n";   break;
    case 2: turn_card();        std::cout << "\tTurn\n";   break;
    case 3: river_card();       std::cout << "\tRiver\n";  break;
    case 4: check_for_winner(); std::cout << "\tCall\n";   break;
    case 5: game_start();                                   break;
    default: break;
    }
    ++stage_;
}

void GameTable::deal()
{
    for (auto& p : host_players_)
        if (!p.who().empty())
            p.new_hand(game_.deal(2));
}

void GameTable::flop()
{
    Cards c = game_.flop();
    common_cards_.insert(common_cards_.end(), c.begin(), c.end());
}

void GameTable::turn_card()
{
    common_cards_.push_back(game_.turn());
}

void GameTable::river_card()
{
    common_cards_.push_back(game_.river());

    for (int k = 0; k < 5; ++k)
    {
        if (host_players_[k].who().empty()) continue;
        final_hand_[k].player_     = boost::make_shared<Player>(host_players_[k]);
        final_hand_[k].final_hand_ = common_cards_ + host_players_[k].call();
    }
}

// ─── Scoring ──────────────────────────────────────────────────────────────────

ScoreBoard GameTable::tabulate(const Cards& hand)
{
    boost::array<int, 14> ranks = {0};
    boost::array<int, 4>  suits = {0};

    for (auto c : hand)
    {
        ++ranks[c.rank - 2];
        ++suits[c.suit - 1];
    }

    int n = 0;
    bool Straight = false;
    for (int r = 1; r < 14; ++r)
    {
        if (ranks[r] && ranks[r - 1]) ++n;
        else n = 0;
        if (n > 4) Straight = true;
    }

    bool Flush = false;
    for (auto s : suits) if (s > 4) Flush = true;

    bool FourKind = false;
    for (auto r : ranks) if (r == 4) FourKind = true;

    bool ThreeKind = false;
    for (auto r : ranks) if (r == 3) ThreeKind = true;

    bool Pair1 = false, TwoPair = false;
    for (auto r : ranks)
    {
        if (r == 2 && Pair1) TwoPair = true;
        if (r == 2) Pair1 = true;
    }

    bool FullHouse = false;
    if (ThreeKind && Pair1 && !FourKind)
    {
        ThreeKind = false; Pair1 = false; FullHouse = true;
    }

    bool StraightFlush = false;
    if (Straight && Flush)
    {
        Straight = false; Flush = false; StraightFlush = true;
    }

    Card HighCard;
    for (auto c : hand)
        if (c.rank > HighCard.rank) HighCard = c;

    return ScoreBoard(StraightFlush, FourKind, FullHouse, Flush,
                      Straight, ThreeKind, TwoPair, Pair1, HighCard);
}

Player* GameTable::check_for_winner()
{
    for (int k = 0; k < 5; ++k)
    {
        if (!final_hand_[k].player_) continue;
        final_hand_[k].final_score_ = tabulate(final_hand_[k].final_hand_);
    }

    int winner_idx = -1;
    int best_tier  = -1;
    for (int k = 0; k < 5; ++k)
    {
        if (!final_hand_[k].player_) continue;
        const auto& sb = final_hand_[k].final_score_;
        int tier = sb.straight_flush_ ? 8 :
                   sb.four_kind_      ? 7 :
                   sb.full_house_     ? 6 :
                   sb.flush_          ? 5 :
                   sb.straight_       ? 4 :
                   sb.three_kind_     ? 3 :
                   sb.two_pair_       ? 2 :
                   sb.one_pair_       ? 1 : 0;
        if (tier > best_tier) { best_tier = tier; winner_idx = k; }
    }

    if (winner_idx >= 0)
        std::cout << "--Winner: " << final_hand_[winner_idx].player_->who() << "--\n";

    return winner_idx >= 0 ? final_hand_[winner_idx].player_.get() : nullptr;
}

ServerPackage GameTable::make_package(const Player& p, bool hb, bool wn, bool sp) const
{
    return ServerPackage(hb, wn, sp,
                         boost::container::string(p.who()),
                         boost::container::string(""),
                         const_cast<Player&>(p).call(),
                         common_cards_);
}

// ─── Heartbeat ────────────────────────────────────────────────────────────────

[[noreturn]] void GameTable::start_heart_beat()
{
    std::cout << "--Starting heartbeat--\n";
    for (;;)
    {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));

        while (!incoming_queue_.empty())
        {
            process_package(incoming_queue_.front());
            incoming_queue_.pop_front();
        }

        for (const auto& entry : seated_players_)
        {
            const uuid& u    = entry.left;
            SeatPtr session  = entry.right;
            Player* p = find_player_by_uuid(u);
            if (!p) continue;

            ServerPackage pkg = make_package(*p, true, false, false);
            std::stringstream ss;
            ss << pkg;

            Message msg;
            std::string body = ss.str();
            msg.allocate_body(body.size());
            std::memcpy(msg.body(), body.c_str(), msg.get_body_length());
            std::memcpy(msg.uuid(), u.data, 16);
            msg.encode_header();

            session->write_message(msg);
        }
    }
}

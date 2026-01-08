// Created by CBunt on 16 May 2020.
//

#include "NetworkController.hpp"

using namespace boost;
using namespace boost::interprocess;
using namespace boost::container;

Table::Table(boost::asio::io_context& c, const tcp::endpoint e) :
    context_(c), socket_(c)
{
    NetworkController Server(c, e, this);
    server_thread_ = make_shared<thread>(
        thread([&c]() { c.run(); }));
    game_table_ = new Game;

    for (int k = 0; k < 5; ++k)
    {
        format Fmt = format("Player%1%") % k;
        new_player(string(Fmt.str().c_str()));
    }
}


[[noreturn]] void Table::start_heart_beat()
{
    std::cout << "--STARTING HEART BEAT--\n";

    Message UpDt;
    string Pack;
    std::stringstream StringBuff;

    for (;;)
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        process_update();
        for (const auto& p : seated_players_)
        {
            Pack = make_package(p.right, true, false, false)->serial();
            UpDt.allocate_body(std::strlen(Pack.c_str()) + 1);
            std::memcpy(UpDt.body(), Pack.c_str(), UpDt.get_body_length());
            UpDt.encode_header();
            p.left->count_timer();
            p.left->signal(UpDt);
        }
    }
}

int Table::new_player(const string& name)
{
    for (int k = 0; k < 5; ++k)
    {
        if (format Fmt = format("Player%1%") % k; Fmt.str().c_str() != host_player_[k].who().c_str())
        {
            host_player_[k].add_player(name);
            std::cout << "--Player: " << name << " Added--\n";
            return k;
        }
    }

    return -1;
}

// Take a pointer to a socket and the first message from that socket
// Pulls the Name out of it and assigning association with player pointer.
int Table::incoming_player(SeatPtr s, Message& u)
{
    ClientPackage Pack(u.body());
    int pos;
    //in >> Pack;

    pos = new_player(Pack.name_);
    if (pos < 0)
        return pos;

    seated_players_.insert(SocketSeatPair(s,
        make_shared<Player>(host_player_[pos])));

    dynamic_cast<Session&>(*s).join();

    return pos;
}

void Table::player_leave(SeatPtr Seat)
{
    if (seated_players_.left.at(Seat))
        seated_players_.left.at(Seat)->fold();
    seated_players_.left.erase(Seat);
}

void Table::incoming_update(Message u)
{
    incoming_queue_.push_back(u);
}


// This can be written more effectively. maybe rewrite to attach the
// socket to the Player class.
void Table::process_update()
{
    while (!incoming_queue_.empty())
    {
        ClientPackage Pack;
        boost::shared_ptr<Message> UpdatePtr = make_shared<Message>(incoming_queue_.front());
        bufferstream BufferStream(UpdatePtr->body(),
                                  UpdatePtr->get_body_length());
        BufferStream >> Pack;

        PlayerPtr Ptr;
        for (int k = 0; k < 5; ++k)
            if (host_player_[k].who() == Pack.name_)
                Ptr = make_shared<Player>(host_player_[k]);
        if (!Ptr.get()) return;

        if (Pack.heart_beat_)
        {
            seated_players_.right.at(Ptr)->reset_timer();
            return;
        }

        if (Pack.next_step_)
        {
            step();
            return;
        }

        if (Pack.leave_)
        {
            player_leave(seated_players_.right.at(Ptr));
            return;
        }
        incoming_queue_.pop_front();
    }
}


void Table::game_start()
{
    delete game_table_;
    game_table_ = new Game;
    stage_ = 0;
}

void Table::step()
{
    switch (stage_)
    {
    case 0:
        deal();
        std::cout << "\tDeal\n";
        return;
    case 1:
        flop();
        std::cout << "\tFlop\n";
        return;
    case 2:
        turn();
        std::cout << "\tTurn\n";
        return;
    case 3:
        river();
        std::cout << "\tRiver\n";
        return;
    case 4:
        check_for_winner();
        std::cout << "\tCall\n";
        return;
    case 5:
        game_start();
        std::cout << "\tNew Game\n";
        return;
    default:
        return;
    }
}

void Table::deal()
{
    for (int k = 0; k < 5; ++k)
        host_player_[k].new_hand(game_table_->deal(2));
}

Cards Table::flop()
{
    Cards ret = game_table_->flop();
    common_cards_ = ret;
    return ret;
}

Card Table::turn()
{
    Card ret = game_table_->river();
    common_cards_.emplace_back(ret);
    return ret;
}

Card Table::river()
{
    Card ret = game_table_->turn();
    common_cards_.emplace_back(ret);
    for (int k = 0; k < 5; ++k)
    {
        if (!final_hand_[k].player_)
            continue;
        final_hand_[k].player_ = make_shared<Player>(host_player_[k]);
        final_hand_[k].final_hand_ = common_cards_ + host_player_[k].call();
    }
    return ret;
}

ScoreBoard Table::tabulate(const Cards& Hand)
{
    array<int, 14> ranks = {0};
    array<int, 4> suits = {0};

    for (auto c : Hand)
    {
        ++ranks[c.rank - 2];
        ++suits[c.suit - 1];
    }
    int n = 0;
    bool Straight = false;
    for (int r = 1; r < 14; ++r)
    {
        if (ranks[r] and ranks[r - 1])
            ++n;
        else n = 0;
        if (n > 4) Straight = true;
    }

    bool Flush = false;
    for (auto s : suits)
        if (s > 4) Flush = true;

    bool FourKind = false;
    for (auto r : ranks)
        if (r == 4) FourKind = true;

    bool ThreeKind = false;
    for (auto r : ranks)
        if (r == 3) ThreeKind = true;

    bool Pair1 = false;
    bool TwoPair = false;
    for (auto r : ranks)
    {
        if (r == 2 and Pair1)
            TwoPair = true;
        if (r == 2) Pair1 = true;
    }

    bool FullHouse = false;
    if (ThreeKind and Pair1 and !FourKind)
    {
        ThreeKind = false;
        Pair1 = false;
        FullHouse = true;
    }

    bool StraightFlush = false;
    if (Straight and Flush)
    {
        Straight = false;
        Flush = false;
        StraightFlush = true;
    }

    Card HighCard;
    for (auto c : Hand)
    {
        if (c.rank < HighCard.rank)
            HighCard = c;
    }
    return ScoreBoard(StraightFlush, FourKind, FullHouse, Flush,
                      Straight, ThreeKind, TwoPair, Pair1, HighCard);
}


Player* Table::check_for_winner()
{
    Player* ret = nullptr;

    for (auto p : final_hand_)
    {
        p.final_score_ = tabulate(p.final_hand_);
    }

    if (final_hand_[0].final_score_.straight_flush_ or final_hand_[1].final_score_.straight_flush_ or
        final_hand_[2].final_score_.straight_flush_ or final_hand_[3].final_score_.straight_flush_ or
        final_hand_[4].final_score_.straight_flush_)
    {
    }

    if (final_hand_[0].final_score_.four_kind_ or final_hand_[1].final_score_.four_kind_ or
        final_hand_[2].final_score_.four_kind_ or final_hand_[3].final_score_.four_kind_ or
        final_hand_[4].final_score_.four_kind_)
    {
    }

    if (final_hand_[0].final_score_.full_house_ or final_hand_[1].final_score_.full_house_ or
        final_hand_[2].final_score_.full_house_ or final_hand_[3].final_score_.full_house_ or
        final_hand_[4].final_score_.full_house_)
    {
    }

    if (final_hand_[0].final_score_.flush_ or final_hand_[1].final_score_.flush_ or
        final_hand_[2].final_score_.flush_ or final_hand_[3].final_score_.flush_ or
        final_hand_[4].final_score_.flush_)
    {
    }

    if (final_hand_[0].final_score_.straight_ or final_hand_[1].final_score_.straight_ or
        final_hand_[2].final_score_.straight_ or final_hand_[3].final_score_.straight_ or
        final_hand_[4].final_score_.straight_)
    {
    }

    if (final_hand_[0].final_score_.three_kind_ or final_hand_[1].final_score_.three_kind_ or
        final_hand_[2].final_score_.three_kind_ or final_hand_[3].final_score_.three_kind_ or
        final_hand_[4].final_score_.three_kind_)
    {
    }

    if (final_hand_[0].final_score_.two_pair_ or final_hand_[1].final_score_.two_pair_ or
        final_hand_[2].final_score_.two_pair_ or final_hand_[3].final_score_.two_pair_ or
        final_hand_[4].final_score_.two_pair_)
    {
    }

    if (final_hand_[0].final_score_.one_pair_ or final_hand_[1].final_score_.one_pair_ or
        final_hand_[2].final_score_.one_pair_ or final_hand_[3].final_score_.one_pair_ or
        final_hand_[4].final_score_.one_pair_)
    {
    }


    return ret;
}

ServerPackage* Table::make_package(PlayerPtr p, bool hb, bool wn, bool sp) const
{
    return new ServerPackage(hb, wn, sp, p->who(), "", p->call(), common_cards_);
}

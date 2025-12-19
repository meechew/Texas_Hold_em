// Created by CBunt on 20 May 2020.
//
// Documentation for this controller can be found at:
// https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio.html

#ifndef TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#define TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>
#include <utility>
#include <stdexcept>
#include <boost/asio.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/bimap.hpp>
#include "Package.hpp"
#include "Update.hpp"
#include "../Player/Player.hpp"
#include "../Deck/Game.hpp"

using boost::asio::ip::tcp;

typedef std::shared_ptr<Player> PlayerPtr;
typedef std::deque<Update> UpdateQueue;
typedef boost::array<Player,5> Players;

class Seat {
public:
  virtual void signal(const Update& u) = 0;
  virtual void reset_timer() = 0;
  virtual void count_timer() = 0;
};

typedef std::shared_ptr<Seat> SeatPtr;

struct ScoreBoard {
  Card high_card_      = Card(0, 0);
  bool straight_flush_ = false;
  bool four_kind_      = false;
  bool full_house_     = false;
  bool flush_          = false;
  bool straight_       = false;
  bool three_kind_     = false;
  bool two_pair_       = false;
  bool one_pair_       = false;
  ScoreBoard()         = default;
  ScoreBoard(bool StraightFlush, bool FourKind, bool FullHouse, bool Flush, bool Straight,
             bool ThreeKind, bool TwoPair, bool OnePair, const Card &HighCard) :
    high_card_(HighCard), straight_flush_(StraightFlush),
    four_kind_(FourKind), full_house_(FullHouse), flush_(Flush), straight_(Straight),
    three_kind_(ThreeKind), two_pair_(TwoPair), one_pair_(OnePair) {}
};

struct Finals {
  PlayerPtr player_;
  Cards final_hand_;
  ScoreBoard final_score_;
};

typedef boost::array<Finals,5> PlayerFinals;
typedef boost::bimap<SeatPtr, PlayerPtr> SocketSeatLink;
typedef SocketSeatLink::value_type SocketSeatPair;

class Table {
private:
  int stage_ = 0;
  Game*                          game_table_ = nullptr;
  Players                        host_player_;
  PlayerFinals                   final_hand_;
  SocketSeatLink                 seated_players_;
  boost::asio::io_context&       context_;
  tcp::socket                    socket_;
  UpdateQueue                    incoming_queue_;
  std::shared_ptr<boost::thread> server_thread_;

  static ScoreBoard tabulate(const Cards&);
  int new_player(const boost::container::string& name);

  [[noreturn]] void start_heart_beat();
  void  deal();
  Cards flop();
  Card  turn();
  Card  river();

public:
  Cards common_cards_;
  Table() = delete;
  Table(boost::asio::io_context& c, tcp::endpoint e);
  void add_thread(std::shared_ptr<boost::thread> t) {
    server_thread_ = std::move(t);
  }
  int  incoming_player(SeatPtr s, Update& u);
  void player_leave(SeatPtr s);
  void incoming_update(Update u);
  void process_update();
  void game_start();
  void step();
  Player* check_for_winner();
  ServerPackage* make_package(PlayerPtr p, bool hb, bool wn, bool sp);

};

class Session:
  public Seat,
  public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket Skt,Table& Tbl)
    : socket_(std::move(Skt)), table_(Tbl) {}
  void start();
  void signal(const Update& Upd) override;
  void join() {joined_ = true;}
  void reset_timer() override {timer_ = 0;}
  void count_timer() override {++timer_;}
private:
  bool        joined_ = false;
  int         timer_ = 0;
  tcp::socket socket_;
  Table&      table_;
  Update      incoming_update_;
  UpdateQueue update_queue_;
  PlayerPtr   seated_player_pointer_;
  void do_read_header(const boost::system::error_code& error_code);
  void do_read_body(const boost::system::error_code& error_code);
  void do_write(const boost::system::error_code& error_code);
};

class NetworkController {
public:
  NetworkController(
      boost::asio::io_context& c,
		  const tcp::endpoint& e,
		  Table* t
		):
    acceptor_(c, e), table_(t){
    do_accept();
  }

private:
  tcp::acceptor acceptor_;
  void do_accept();
  Table* table_;
};


#endif //TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP

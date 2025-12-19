// Created by CBunt on 17 June 2020.
//

#ifndef TEXAS_HOLD_EM_SERVER_HPP
#define TEXAS_HOLD_EM_SERVER_HPP
#include <deque>
#include <cstdio>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/container/map.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/string.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/container/container_fwd.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

#include "Update.hpp"
#include "../Deck/Game.hpp"

using boost::asio::ip::tcp;
using string = boost::container::string;


class Seat {
public:
  virtual void signal(const Update& u) = 0;
  virtual void reset_timer() = 0;
  virtual void count_timer() = 0;
};

typedef std::shared_ptr<Seat> SeatPtr;

class Player {
private:
  string  name_{""};
  Hand    hand_;
  SeatPtr session_;
public:
  Player() = default;
  explicit Player(string n):
      name_(std::move(n)) {}
  void new_hand(Cards Dealt) {
    hand_.first = Dealt.front();
    hand_.second = Dealt.back();
  }
  
  void add_player(string n) {
    name_ = std::move(n);
  }
  
  void fold() {
    hand_ = std::make_pair<Card, Card>(Card(0, 0), Card(0, 0));
  }
  
  string who() {
    return name_;
  }
  
  Hand call() {
    return hand_;
  }
};

class ServerPackage {
public:
  bool   heart_beat_    = false;
  bool   winner_notice_ = false;
  bool   split_pot_     = false;
  string name_;
  string winner_;
  Hand   hand_;
  boost::container::vector<Card> table_;
  ServerPackage() = default;
  ServerPackage(char* UpDt);
  ServerPackage(bool hb, bool wn, bool sp, string n, string w, Hand h,
                boost::container::vector<Card> tbl):
      heart_beat_(hb), winner_notice_(wn), split_pot_(sp), name_(std::move(n)),
      winner_(std::move(w)), hand_(std::move(h)), table_(std::move(tbl)){}
  string serial();
  friend std::ostream &operator<<(std::ostream &out, const ServerPackage &s);
  friend std::istream &operator>>(std::istream &in, ServerPackage &s);
};

class ClientPackage {
public:
  bool   heartbeat_ = false;
  bool   next_step_  = false;
  bool   leave_     = false;
  string name_;
  ClientPackage() = default;
  ClientPackage(char* UpDt);
  ClientPackage(bool hb, bool ng, bool l, string n):
      heartbeat_(hb), next_step_(ng), leave_(l), name_(std::move(n)){}
  friend std::ostream &operator<<(std::ostream &out, const ClientPackage &s);
  friend std::istream &operator>>(std::istream &in, ClientPackage &s);
  string serial();
};


typedef std::shared_ptr<Player> PlayerPtr;
typedef std::deque<Update> UpdateQueue;
typedef boost::array<Player,5> Players;

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
	     bool ThreeKind, bool TwoPair, bool OnePair, const Card &HighCard) : high_card_(HighCard), straight_flush_(StraightFlush),
										 four_kind_(FourKind), full_house_(FullHouse), flush_(Flush), straight_(Straight),
										 three_kind_(ThreeKind), two_pair_(TwoPair), one_pair_(OnePair) {}
};

struct Finals {
  PlayerPtr  player_;
  Cards      final_hand_;
  ScoreBoard final_score_;
};

typedef boost::array<Finals,5> PlayerFinals;

class Table {
private:
  int          stage_ = 0;
  Game*        game_table_ = nullptr;
  Players      host_player_;
  PlayerFinals final_hand_;

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
  Table(boost::asio::io_context &c, tcp::endpoint e);
  void add_thread(std::shared_ptr<boost::thread> t) {
    server_thread_ = std::move(t);
  }
  int incoming_player(SeatPtr s, Update& u);
  void player_leave(SeatPtr);
  void incoming_update(Update u);
  void process_update();
  void game_start();
  void step();
  Player* check_for_winner();
  ServerPackage *make_package(PlayerPtr p, bool hb, bool wn, bool sp);

};

class Session:
  public Seat,
  public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket Skt,Table& Tbl)
    : socket_(std::move(Skt)), table_(Tbl) {}
  void start();
  void signal (const Update& Upd) override;
  void join() {joined_ = true;}
  void reset_timer() override {timer_ = 0;}
  void count_timer() override {++timer_;}
private:
  int timer_ = 0;
  tcp::socket socket_;
  Table& table_;
  Update incoming_update_;
  bool joined_ = false;
  UpdateQueue update_queue_;
  void do_read_header(const boost::system::error_code& error_code);
  void do_read_body(const boost::system::error_code& error_code);
  void do_write(const boost::system::error_code& error_code);
  PlayerPtr seated_player_pointer_;
};

class NetworkController {
public:
  NetworkController(boost::asio::io_context& c,
		    const tcp::endpoint& e, Table* t):
    acceptor_(c, e), table_(t){
    do_accept();
  }

private:
  tcp::acceptor acceptor_;
  void do_accept();
  Table* table_;
};

#endif

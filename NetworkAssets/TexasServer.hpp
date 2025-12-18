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
  ServerPackage(bool hb, bool wn, bool sp, string n, string w, hand h,
                boost::container::vector<card> tbl):
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
	     bool ThreeKind, bool TwoPair, bool OnePair, const card &HighCard) : high_card_(HighCard), straight_flush_(StraightFlush),
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
  int Stage = 0;
  Game *TableGame = nullptr;
  Players HostPlayers;
  PlayerFinals FinalHands;

  boost::asio::io_context& ioContext;
  tcp::socket Socket;
  UpdateQueue IncomingQueue;
  std::shared_ptr<boost::thread> ServerThread;

  static ScoreBoard Tabulate(const cards&);
  int NewPlayer(const boost::container::string& name);

  [[noreturn]] void StartHeartBeat();
  void Deal();
  cards Flop();
  card Turn();
  card River();

public:
  cards CommonCards;
  Table() = default;
  Table(boost::asio::io_context &Context, tcp::endpoint Endpoint);
  void AddThread(std::shared_ptr<boost::thread> ThrPtr) {
    ServerThread = std::move(ThrPtr);
  }
  int IncomingPlayer(SeatPtr Seat, Update& UpDt);
  void PlayerLeave(SeatPtr);
  void IncomingUpdate(Update UpDt);
  void ProcessUpdate();
  void GameStart();
  void Step();
  Player* CheckForWinner();
  ServerPackage *Package(PlayerPtr p, bool hb, bool wn, bool sp);

};

class Session:
  public Seat,
  public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket Skt,Table& Tbl)
    : Skt(std::move(Skt)), Tbl(Tbl) {}
  void Start();
  void signal (const Update& Upd) override;
  void Join() {Joined = true;}
  void reset_timer() override {Timer = 0;}
  void count_timer() override {++Timer;}
private:
  int Timer = 0;
  tcp::socket Skt;
  Table& Tbl;
  Update ReadUpdate;
  bool Joined = false;
  UpdateQueue WriteUpdate;
  void DoReadHeader(const boost::system::error_code& error_code);
  void DoReadBody(const boost::system::error_code& error_code);
  void DoWrite(const boost::system::error_code& error_code);
  PlayerPtr PtrToSeatedPlayer;
};

class NetworkController {
public:
  NetworkController(boost::asio::io_context& Context,
		    const tcp::endpoint& Endpoint, Table* TblPtr):
    Acceptor(Context, Endpoint), Tbl(TblPtr){
    DoAccept();
  }

private:
  tcp::acceptor Acceptor;
  void DoAccept();
  Table* Tbl;
};

#endif

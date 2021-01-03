// Created by CBunt on 17 June 2020.
//

#ifndef TEXAS_HOLD_EM_SERVER_HPP
#define TEXAS_HOLD_EM_SERVER_HPP
#include <deque>
#include <cstdio>
#include <memory>
#include <chrono>
#include <utility>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
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
#include "Game.hpp"

using boost::asio::ip::tcp;
using string = boost::container::string;


class Seat {
public:
  virtual void Signal(const Update& Upd) = 0;
  virtual void ResetTimer() = 0;
  virtual void CountTimer() = 0;
};

typedef std::shared_ptr<Seat> SeatPtr;

class Player {
private:
  string Name{""};
  hand Hand;
  SeatPtr ClientSession;
public:
  Player() = default;
  explicit Player(string n):
      Name(std::move(n)) {}
  void NewHand(cards Dealt) {
    Hand.first = Dealt.front();
    Hand.second = Dealt.back();
  }
  
  void AddPlayer(string n) {
    Name = std::move(n);
  }
  
  void Fold() {
    Hand = std::make_pair<card,card>(card(0, 0), card(0, 0));
  }
  
  string Who() {
    return Name;
  }
  
  hand Call() {
    return Hand;
  }
};

class ServerPackage {
public:
  bool HeartBeat = false;
  bool WinnerNotice = false;
  bool SplitPot = false;
  string Name;
  string Winner;
  hand Hand;
  boost::container::vector<card> Tbl;
  ServerPackage() = default;
  ServerPackage(char* UpDt);
  ServerPackage(bool hb, bool wn, bool sp, string n, string w, hand h,
                boost::container::vector<card> tbl):
      HeartBeat(hb), WinnerNotice(wn), SplitPot(sp), Name(std::move(n)),
      Winner(std::move(w)), Hand(std::move(h)), Tbl(std::move(tbl)){}
  string Serial();
  friend std::ostream &operator<<(std::ostream &out, const ServerPackage &s);
  friend std::istream &operator>>(std::istream &in, ServerPackage &s);
};

class ClientPackage {
public:
  bool HeartBeat = false;
  bool NextStep = false;
  bool Leave = false;
  string Name;
  ClientPackage() = default;
  ClientPackage(char* UpDt);
  ClientPackage(bool hb, bool ng, bool l, string n):
      HeartBeat(hb), NextStep(ng), Leave(l), Name(std::move(n)){}
  friend std::ostream &operator<<(std::ostream &out, const ClientPackage &s);
  friend std::istream &operator>>(std::istream &in, ClientPackage &s);
  string Serial();
};


typedef std::shared_ptr<Player> PlayerPtr;
typedef std::deque<Update> UpdateQueue;
typedef boost::array<Player,5> Players;

struct ScoreBoard {
  card HighCard = card(0, 0);
  bool StraightFlush = false;
  bool FourKind = false;
  bool FullHouse = false;
  bool Flush = false;
  bool Straight = false;
  bool ThreeKind = false;
  bool TwoPair = false;
  bool OnePair = false;
  ScoreBoard() = default;
  ScoreBoard(bool StraightFlush, bool FourKind, bool FullHouse, bool Flush, bool Straight,
	     bool ThreeKind, bool TwoPair, bool OnePair, const card &HighCard) : HighCard(HighCard), StraightFlush(StraightFlush),
										 FourKind(FourKind), FullHouse(FullHouse), Flush(Flush), Straight(Straight),
										 ThreeKind(ThreeKind), TwoPair(TwoPair), OnePair(OnePair) {}
};

struct Finals {
  PlayerPtr player;
  cards FinalHand;
  ScoreBoard FinalScore;
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
  void Signal (const Update& Upd) override;
  void Join() {Joined = true;}
  void ResetTimer() override {Timer = 0;}
  void CountTimer() override {++Timer;}
private:
  int Timer = 0;
  tcp::socket Skt;
  Table& Tbl;
  Update ReadUpdate;
  bool Joined = false;
  UpdateQueue WriteUpdate;
  void DoReadHeader();
  void DoReadBody();
  void DoWrite();
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

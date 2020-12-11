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
#include "Player.hpp"
#include "Update.hpp"
#include "Game.hpp"
#include "Package.hpp"

using boost::asio::ip::tcp;

typedef std::shared_ptr<Player> PlayerPtr;
typedef std::deque<Update> UpdateQueue;
typedef boost::array<Player,5> Players;

class Seat {
public:
  virtual void Signal(const Update& Upd) = 0;
  virtual void ResetTimer() = 0;
  virtual void CountTimer() = 0;
};

typedef std::shared_ptr<Seat> SeatPtr;

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
typedef boost::bimap<SeatPtr, PlayerPtr> SocketSeatLink;
typedef SocketSeatLink::value_type SocketSeatPair;

class Table {
private:
  int Stage = 0;
  Game *TableGame = nullptr;
  Players HostPlayers;
  PlayerFinals FinalHands;
  SocketSeatLink SeatedPlayers;
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
  void ProssesUpdates();
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


#endif //TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP

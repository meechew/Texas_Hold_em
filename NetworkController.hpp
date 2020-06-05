// Created by CBunt on 20 May 2020.
//
// Documentation for this controller can be found at:
// https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio.html

#ifndef TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#define TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#include <cstdlib>
#include <deque>
#include <iostream>
//#include <list>
//#include <set>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <local/include/boost/array.hpp>
#include <local/include/boost/thread.hpp>
#include <boost/format.hpp>
#include "Player.hpp"
#include "Update.hpp"
#include "Game.hpp"
#include "Package.hpp"

using boost::asio::ip::tcp;

typedef std::deque<Update> UpdateQueue;

class Seat {
public:
  virtual void Signal(const Update& Upd) = 0;
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

typedef std::shared_ptr<Player> PlayerPtr;

struct Finals {
  PlayerPtr player;
  cards FinalHand;
  ScoreBoard FinalScore;
};

typedef boost::array<Finals,5> PlayerFinals;
typedef boost::array<Player,5> Players;


class Table {
private:
  Game *TableGame = nullptr;
  Players HostPlayers;
  PlayerFinals FinalHands;
  static ScoreBoard Tabulate(const cards&);
  int NewPlayer(boost::container::string name);
  boost::thread TheadStarter();
  boost::container::map<SeatPtr, PlayerPtr> SeatedPlayers;
  std::shared_ptr<boost::thread> ServerThread;

public:
  cards CommonCards;
  Table();
  int IncomingPlayer(SeatPtr);
  void PlayerLeave(SeatPtr);
  void IncomingUpdate(Update);
  void GameStart();
  void Deal();
  cards Flop();
  card River();
  card Turn();
  Player* CheckForWinner();
  ServerPackage *Package(int n, bool hb, bool wn, bool sp);
};

/*class TableServer {
public:
  void Join(SeatPtr SeatedPlayer);
  void Leave(SeatPtr SeatedPlayer) {Seats.erase(SeatedPlayer);}
  void Signal(const Update& Upd);
  TableServer(std::shared_ptr<Table> TblPtr): TblPtr(TblPtr) {}
private:
  std::set<SeatPtr> Seats;
  enum {MaxQueue = 100};
  UpdateQueue RecentUpdates;
};*/

class Session:
  public Seat,
  public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket Skt,Table& Tbl)
    : Socket(std::move(Skt)), Tbl(Tbl) {}
  void Start();
  void Signal (const Update& Upd);
private:
  tcp::socket Socket;
  Table& Tbl;
  Update ReadUpdate;
  UpdateQueue WriteUpdate;
  enum {max_length = 1024};
  char Data[max_length];

  void DoRead();
  void DoReadHeader();
  void DoReadBody();
  void DoWrite();
};

class NetworkController {
public:
  NetworkController(boost::asio::io_context& Context,
		    const tcp::endpoint& Endpoint):
    Acceptor(Context, Endpoint) {
    DoAccept();
  }

private:
  void DoAccept();
  tcp::acceptor Acceptor;
  Table Tbl;
};


#endif //TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP

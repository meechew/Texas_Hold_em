// Created by CBunt on 20 May 2020.
//
// Documentation for this controller can be found at:
// https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio.html

#ifndef TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#define TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "Player.hpp"
#include "Update.hpp"

using boost::asio::ip::tcp;

typedef std::deque<Update> UpdateQueue;

class Seat {
public:
  virtual void Signal(const Update& Upd) = 0;
};

typedef std::shared_ptr<Seat> SeatPtr;

class TableServer {
public:
  void Join(SeatPtr SeatedPlayer) {}
  void Leave(SeatPtr SeatedPlayer) {Seats.erase(SeatedPlayer);}
  void Signal(const Update& Upd) {
    RecentUpdates.push_back(Upd);
    while (RecentUpdates.size() > MaxQueue)
      RecentUpdates.pop_front();

    for (auto seat : Seats)
      seat->Signal(Upd);
  }

private:
  std::set<SeatPtr> Seats;
  enum {MaxQueue = 100};
  UpdateQueue RecentUpdates;
};

class Session:
  public Seat,
  public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket Skt,TableServer& Tbl)
    : Socket(std::move(Skt)), Tbl(Tbl) {}
  void Start();
  void Signal (const Update& Upd);
private:
  tcp::socket Socket;
  TableServer Tbl;
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
  TableServer Tbl;
};


#endif //TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP

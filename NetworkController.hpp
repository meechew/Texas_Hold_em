// Created by CBunt on 20 May 2020.
//
// Documentation for this controller can be found at:
// https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio.html

#ifndef TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#define TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session: public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket socket)
      : socket_(std::move(socket)) {}
  void start() {do_read();}
private:
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];

  void do_read();
  void do_write(std::size_t length);

};

class NetworkController {
public:
  NetworkController(boost::asio::io_context& io_context, short port)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
        {do_accept();}

private:
  void do_accept();
  tcp::acceptor acceptor_;
};

#endif //TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP

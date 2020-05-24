// Created by CBunt on 20 May 2020.
//

#include "NetworkController.hpp"

void NetworkController::do_accept() {
  acceptor_.async_accept(
    [this](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        std::make_shared<Session>(std::move(socket))->start();
      }
      do_accept();
    });
}

void Session::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(data_, max_length),
    [this, self](boost::system::error_code ec, std::size_t length) {
      if (!ec) {
        do_write(length);
      }
    });
}

void Session::do_write(std::size_t length) {
  auto self(shared_from_this());
  boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
    [this, self](boost::system::error_code ec, std::size_t /*length*/) {
      if (!ec) {
       do_read();
      }
    });
}


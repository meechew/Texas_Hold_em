// Created by CBunt on 20 May 2020.
//

#include "NetworkController.hpp"

void Session::Start() {
  Tbl.Join(shared_from_this());
  DoRead();
}

void Session::Deliver(const Update& Upd) {
  bool WriteInProgress = !WriteUpdate.empty();
  WriteUpdate.push_back(Upd);
  if (!WriteInProgress)
    DoWrite();
}

void Session::DoRead() {

}

void Session::DoReadHeader() {
  auto self(shared_from_this());
  boost::asio::async_read(Socket,
      boost::asio::buffer(ReadUpdate.data(), Update::header_length),
      [this, self](boost::system::error_code ErrorCode, std::size_t) {
        if (!ErrorCode && ReadUpdate.decode_header()) {
          DoReadBody();
        }
        else {
          Tbl.Leave(shared_from_this());
        }
      });

}

void Session::DoReadBody() {
  auto self(shared_from_this());
  boost::asio::async_read(Socket,
                          boost::asio::buffer(ReadUpdate.body(), ReadUpdate.body_length()),
                          [this, self](boost::system::error_code ErrorCode, std::size_t) {
                            if (!ErrorCode) {
                              Tbl.Signal(ReadUpdate);
                              DoReadHeader();
                            }
                            else {
                              Tbl.Leave(shared_from_this());
                            }
                          });
}

void Session::DoWrite() {
  auto self(shared_from_this());
  boost::asio::async_write(Socket,
      boost::asio::buffer(WriteUpdate.front().data(),
          WriteUpdate.front().length()),
          [this, self](boost::system::error_code ErrorCode,
              std::size_t)
                           {
    if (!ErrorCode) {
      WriteUpdate.pop_front();
      if (!WriteUpdate.empty()) {
        DoWrite();
      }
    }
    else {
      Tbl.Leave(shared_from_this());
    }
                           })
}



void NetworkController::DoAccept() {
  Acceptor.async_accept(
    [this](boost::system::error_code ErrorCode, tcp::socket Socket) {
      if (ErrorCode) {
        std::make_shared<Session>(std::move(Socket), Tbl)->Start();
      }
      DoAccept();
    });
}




/*void NetworkController::do_accept() {
  acceptor_.async_accept(
    [this](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        std::make_shared<Session>(std::move(socket))->Start();
      }
      do_accept();
    });
}

void Session::do_read() {
  auto self(shared_from_this());
  Socket.async_read_some(boost::asio::buffer(data_, max_length),
                         [this, self](boost::system::error_code ec, std::size_t length) {
      if (!ec) {
        do_write(length);
      }
    });
}

void Session::do_write(std::size_t length) {
  auto self(shared_from_this());
  boost::asio::async_write(Socket, boost::asio::buffer(data_, length),
                           [this, self](boost::system::error_code ec, std::size_t *//*length*//*) {
      if (!ec) {
       do_read();
      }
    });
}*/


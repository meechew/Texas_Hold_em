// Created by CBunt on 20 May 2020.
//

#include "NetworkController.hpp"

void Session::Start() {
  Tbl.Join(shared_from_this());
  DoRead();
}

void Session::Signal(const Update& Upd) {
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
			  boost::asio::buffer(ReadUpdate.data(), Update::HeaderLength),
			  [this, self](boost::system::error_code ErrorCode, std::size_t) {
			    if (!ErrorCode && ReadUpdate.MakeHeader()) {
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
                          boost::asio::buffer(ReadUpdate.Body(),
					      ReadUpdate.RetBodyLength()),
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
			   });
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







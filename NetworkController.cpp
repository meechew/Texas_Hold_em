// Created by CBunt on 20 May 2020.
//

#include "TexasServer.hpp"

void Session::Start() {
  auto self(shared_from_this());
  boost::asio::async_read(Skt,
    boost::asio::buffer(ReadUpdate.RetData(), Update::MaxBodyLength),
    [this, self](boost::system::error_code ErrorCode, std::size_t) {
      std::cerr << "--New Connection--\n"
                /* << ReadUpdate.Body()*/ << std::endl;
      if (Tbl.IncomingPlayer(self, ReadUpdate) > 0)
        Joined = true;
  });
  DoReadHeader();
}

void Session::Signal(const Update& Upd) {
  bool WriteInProgress = !WriteUpdate.empty();
  WriteUpdate.push_back(Upd);
  if (!WriteInProgress)
    DoWrite();
}

void Session::DoReadHeader() {
  auto self(shared_from_this());
  boost::asio::async_read(Skt,
			  boost::asio::buffer(ReadUpdate.RetData(), Update::HeaderLength),
			  [this, self](boost::system::error_code ErrorCode, std::size_t) {
			    if (!ErrorCode && ReadUpdate.MakeHeader()) {
			      DoReadBody();
			    }
			    else {
			      Tbl.PlayerLeave(shared_from_this());
			    }
			  });

}

void Session::DoReadBody() {
  auto self(shared_from_this());
  boost::asio::async_read(Skt,
        boost::asio::buffer(ReadUpdate.Body(),
          ReadUpdate.RetBodyLength()),
        [this, self](boost::system::error_code ErrorCode, std::size_t) {
          if (!ErrorCode) {
            if (Joined)
              Tbl.IncomingUpdate(ReadUpdate);
            DoReadHeader();
          }
          else {
            Tbl.PlayerLeave(shared_from_this());
          }
        });
}

void Session::DoWrite() {
  auto self(shared_from_this());
  boost::asio::async_write(Skt,
			   boost::asio::buffer(WriteUpdate.front().RetData(),
                             WriteUpdate.front().Length()),
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
			       Tbl.PlayerLeave(shared_from_this());
			     }
			   });
}

void NetworkController::DoAccept() {
  Acceptor.async_accept(
			[this](boost::system::error_code ErrorCode, tcp::socket Socket) {
			  if (!ErrorCode) {
			    std::make_shared<Session>(std::move(Socket), *Tbl)->Start();
			  }
			  DoAccept();
			});
}







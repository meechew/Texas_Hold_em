// Created by CBunt on 20 May 2020.
//

#include "TexasServer.hpp"

void Session::Start()
{
    std::cerr << "--New Connection--\n"
        /* << ReadUpdate.Body()*/ << std::endl;
    boost::asio::async_read(
        Skt,
        boost::asio::buffer( ReadUpdate.Header(), Update::MaxBodyLength),
        boost::bind(&Session::DoReadHeader, shared_from_this(), boost::asio::placeholders::error)
    );
}

void Session::Signal(const Update& Upd)
{
    bool WriteInProgress = !WriteUpdate.empty();
    WriteUpdate.push_back(Upd);
    if (!WriteInProgress)
        boost::asio::async_write(
            Skt,
            boost::asio::buffer(WriteUpdate.front().Body(), WriteUpdate.front().Length()),
            boost::bind(&Session::DoWrite, shared_from_this(), boost::asio::placeholders::error)
        );
}

void Session::DoReadHeader(const boost::system::error_code& error_code)
{
    if (!error_code && ReadUpdate.DecodeHeader())
    boost::asio::async_read(
        Skt,
        boost::asio::buffer(ReadUpdate.Body(), ReadUpdate.RetBodyLength()),
        boost::bind(&Session::DoReadBody, shared_from_this(), boost::asio::placeholders::error)
    );
    else Tbl.PlayerLeave(shared_from_this());
}

void Session::DoReadBody(const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        if (!Joined)
        {
            Tbl.IncomingPlayer(shared_from_this(), ReadUpdate);
            Joined = true;
        }
        Tbl.IncomingUpdate(ReadUpdate);
        boost::asio::async_read(
            Skt,
            boost::asio::buffer(ReadUpdate.Header(), Update::HeaderLength),
            boost::bind(&Session::DoReadHeader, shared_from_this(), boost::asio::placeholders::error)
        );
    }
    else Tbl.PlayerLeave(shared_from_this());
}

void Session::DoWrite(const boost::system::error_code& error)
{
    auto self(shared_from_this());
    if (!error) {
        WriteUpdate.pop_front();
        if (!WriteUpdate.empty()) {
            boost::asio::async_write(
            Skt,
            boost::asio::buffer(WriteUpdate.front().Header(),WriteUpdate.front().Length()),
            boost::bind(&Session::DoWrite, self, boost::asio::placeholders::error)
            );
        }
    }
    else {
        Tbl.PlayerLeave(self);
    }
}

void NetworkController::DoAccept()
{
    Acceptor.async_accept(
        [this](boost::system::error_code ErrorCode, tcp::socket Socket)
        {
            if (!ErrorCode)
            {
                std::make_shared<Session>(std::move(Socket), *Tbl)->Start();
            }
            DoAccept();
        });
}

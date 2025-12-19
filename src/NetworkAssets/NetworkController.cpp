// Created by CBunt on 20 May 2020.
//

#include "TexasServer.hpp"

void Session::start()
{
    std::cerr << "--New Connection--\n"
        /* << ReadUpdate.Body()*/ << std::endl;
    boost::asio::async_read(
        socket_,
        boost::asio::buffer( incoming_update_.header(), Update::HEADER_LENGTH),
        boost::bind(&Session::do_read_header, shared_from_this(), boost::asio::placeholders::error)
    );
}

void Session::signal(const Update& Upd)
{
    bool WriteInProgress = !update_queue_.empty();
    update_queue_.push_back(Upd);
    if (!WriteInProgress)
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(update_queue_.front().body(), update_queue_.front().length()),
            boost::bind(&Session::do_write, shared_from_this(), boost::asio::placeholders::error)
        );
}

void Session::do_read_header(const boost::system::error_code& error_code)
{
    if (!error_code && incoming_update_.decode_header())
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(incoming_update_.body(), incoming_update_.get_body_length()),
        boost::bind(&Session::do_read_body, shared_from_this(), boost::asio::placeholders::error)
    );
    else table_.player_leave(shared_from_this());
}

void Session::do_read_body(const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        if (!joined_)
        {
            table_.incoming_player(shared_from_this(), incoming_update_);
            joined_ = true;
        }
        table_.incoming_update(incoming_update_);
        boost::asio::async_read(
            socket_,
            boost::asio::buffer(incoming_update_.header(), Update::HEADER_LENGTH),
            boost::bind(&Session::do_read_header, shared_from_this(), boost::asio::placeholders::error)
        );
    }
    else table_.player_leave(shared_from_this());
}

void Session::do_write(const boost::system::error_code& error_code)
{
    auto self(shared_from_this());
    if (!error_code) {
        update_queue_.pop_front();
        if (!update_queue_.empty()) {
            boost::asio::async_write(
            socket_,
            boost::asio::buffer(update_queue_.front().header(),update_queue_.front().length()),
            boost::bind(&Session::do_write, self, boost::asio::placeholders::error)
            );
        }
    }
    else {
        table_.player_leave(self);
    }
}

void NetworkController::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ErrorCode, tcp::socket Socket)
        {
            if (!ErrorCode)
            {
                std::make_shared<Session>(std::move(Socket), *table_)->start();
            }
            do_accept();
        });
}

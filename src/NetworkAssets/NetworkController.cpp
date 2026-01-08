// Created by CBunt on 20 May 2020.
//

#include "NetworkController.hpp"

#include "../GameTable/GameTable.hpp"

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

void Session::process_messages()
{
    while (!incoming_queue_.empty())
    {
        auto _message = incoming_queue_.front();
        incoming_queue_.pop_front();
        if (!seated_player_pointer_)
        {
           // table_.incoming_player(shared_from_this(), _package);
        }

        // table_.process_package(_package);
    }
}

void Session::start()
{
    std::cerr << "--New Sever--\n" << std::endl;
    do_read();
}

void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(read_buffer_),
        [this, self](const boost::system::error_code& ec, std::size_t s) {
        if (!ec) {
            auto* _message = new Message;
            memcpy(_message->data(), read_buffer_, s);
            _message->decode_header();
            incoming_queue_.push_back(ClientPackage(_message->body()));
            delete _message;
            do_read();
        }
    });
}

void Session::do_write(std::size_t s)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_,
                             boost::asio::buffer(read_buffer_, s),
                             [this, self](boost::system::error_code ec, std::size_t) {
        if (!ec) {
            do_read();
        }
    });
}

void NetworkController::start_accept() {
    // Start an asynchronous accept operation
    acceptor_.async_accept(socket_, [this](const boost::system::error_code& ec) {
        handle_accept(ec);
    });
}

void NetworkController::handle_accept(const boost::system::error_code& ec)
{
    if (!ec) {
        std::cout << "Accepted new connection!" << std::endl;
        session_ = boost::make_shared<Session>(std::move(socket_));
        session_->start();
        start_accept();
    }
}

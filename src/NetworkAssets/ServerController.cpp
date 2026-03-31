// Created by meechew on 1/19/26.
//

#include "../GameTable/GameTable.hpp"

void ServerController::start()
{
    if (!is_session_)
    {
        // Listener mode: open acceptor and begin accepting connections
        tcp::endpoint endpoint(tcp::v4(), std::atoi(port_.c_str()));
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
        accept_connections();
    }
    else
    {
        // Session mode: begin reading from the connected socket
        read_message();
    }
}

void ServerController::accept_connections()
{
    auto new_session = ServerController::create_session(io_context_, game_table_);
    auto self(shared_from_this());

    acceptor_.async_accept(new_session->socket_,
        [this, self, new_session](const boost::system::error_code& ec) {
            if (!ec)
            {
                new_session->start();
                accept_connections();
            }
        });
}

void ServerController::on_message_received(const Message& msg)
{
    if (game_table_)
        game_table_->incoming_update(msg, shared_from_this());
}
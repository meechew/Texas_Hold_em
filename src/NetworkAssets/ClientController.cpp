// Created by meechew on 1/14/26.
//

#include "NetworkController.hpp"

void ClientController::start()
{
    tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(host_, port_);
    connect_to_server(endpoints);
}

void ClientController::connect_to_server(const tcp::resolver::results_type& endpoints)
{
    auto self(shared_from_this());
    boost::asio::async_connect(socket_, endpoints,
        [this, self](const boost::system::error_code& ec, const tcp::endpoint&) {
            if (!ec)
            {
                read_message();
            }
        });
}

void ClientController::set_host(std::string h)
{
    host_ = std::move(h);
}

void ClientController::set_port(std::string p)
{
    port_ = std::move(p);
}
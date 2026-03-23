// Created by meechew on 1/14/26.
//

#include "NetworkController.hpp"

void ClientController::start()
{
    tcp::resolver resolver(socket_.get_executor());
    auto _endpoints = resolver.resolve(host, port);
    connect_to_server(_endpoints);
}

void ClientController::connect_to_server(const tcp::resolver::results_type& e)
{
    auto self(shared_from_this());
    boost::asio::async_connect(socket_, e,
        [this, self](const boost::system::error_code& ec, const tcp::endpoint&) {
            if (!ec) {
                // Successfully connected to server
                read_message();
            } else {
                // Handle error
            }
        });
}

void ClientController::set_host(const std::string h)
{
    host = h;
}

void ClientController::set_port(const std::string p)
{
    port = p;
}
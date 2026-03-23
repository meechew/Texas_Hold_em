// Created by meechew on 1/19/26.
//

#include "NetworkController.hpp"

void ServerController::start()
{
    tcp::endpoint _endpoint(boost::asio::ip::tcp::v4(), atoi(port.c_str()));
    acceptor_.open(_endpoint.protocol());
    acceptor_.set_option(tcp::acceptor::reuse_address(true));
    acceptor_.bind(_endpoint);
    acceptor_.listen();

    accept_connections();
}

void ServerController::accept_connections()
{
    auto new_session = create(static_cast<boost::asio::io_context&>(socket_.get_executor().context()));
    auto self(shared_from_this());

    acceptor_.async_accept(new_session->socket_,
        [this, self, new_session](const boost::system::error_code& ec) {
            if (!ec) {
                new_session->start();
                accept_connections(); // Continue accepting new connections
            } else {
                // Handle error
            }
        });
}

void ServerController::set_port(const std::string p)
{
    port = p;
}
// Created by meechew on 1/19/26.
//

#include "NetworkController.hpp"

void NetworkController::read_message()
{
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
                            boost::asio::buffer(current_msg_.header(), Message::HEADER_LENGTH),
                            [this, self](const boost::system::error_code& ec, std::size_t) {
                                handle_read_header(ec);
                            });
}

void NetworkController::write_message(const Message& msg)
{
    auto self(shared_from_this());
    message_queue_.push_back(msg);

    boost::asio::async_write(socket_,
                             boost::asio::buffer(message_queue_.front().data(), message_queue_.front().length()),
                             [this, self](const boost::system::error_code& ec, std::size_t) {
                                 handle_write(ec);
                                 message_queue_.pop_front();
                             });
}

void NetworkController::handle_read_header(const boost::system::error_code& ec)
{
    if (!ec && current_msg_.decode_header())
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(current_msg_.body(), current_msg_.get_body_length()),
                                [this](const boost::system::error_code& ec, std::size_t) {
                                    handle_read_body(ec);
                                });
    }
    else
    {
        // Handle error
    }
}

void NetworkController::handle_read_body(const boost::system::error_code& ec)
{
    if (!ec)
    {
        // Assume UUID is correctly placed in the message
        // Process message using UUID to find session

        read_message(); // To read next message
    }
    else
    {
        // Handle error
    }
}

void NetworkController::handle_write(const boost::system::error_code& ec)
{
    if (!ec)
    {
        if (!message_queue_.empty())
        {
            write_message(message_queue_.front());
        }
    }
    else
    {
        // Handle error
    }
}
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
    bool write_in_progress = !message_queue_.empty();
    message_queue_.push_back(msg);
    if (!write_in_progress)
        do_write();
}

void NetworkController::do_write()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_,
                             boost::asio::buffer(message_queue_.front().data(), message_queue_.front().length()),
                             [this, self](const boost::system::error_code& ec, std::size_t) {
                                 handle_write(ec);
                             });
}

void NetworkController::handle_read_header(const boost::system::error_code& ec)
{
    if (!ec && current_msg_.decode_header())
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(current_msg_.body(), current_msg_.get_body_length()),
                                [this, self](const boost::system::error_code& ec, std::size_t) {
                                    handle_read_body(ec);
                                });
    }
}

void NetworkController::handle_read_body(const boost::system::error_code& ec)
{
    if (!ec)
    {
        on_message_received(current_msg_);
        read_message();
    }
}

void NetworkController::handle_write(const boost::system::error_code& ec)
{
    if (!ec)
    {
        message_queue_.pop_front();
        if (!message_queue_.empty())
            do_write();
    }
}
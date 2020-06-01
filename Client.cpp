// Created by CBunt on 28 May 2020.
//

#ifndef TEXAS_HOLD_EM_CLIENT_CPP
#define TEXAS_HOLD_EM_CLIENT_CPP
// #include <iostream>
// #include <vector>
// #include <boost/asio.hpp>

// std::vector<char> buff(256);

// void ReadHandler(boost::system::error_code ex){
//   std::cout << " print the buffer RetData..." << std::endl;
//   std::cout << buff.RetData() << std::endl;

// }

// void Client(char* IP) {
//   boost::asio::io_service service;
//   using namespace boost::asio::ip;
//   tcp::endpoint endpoint(address::from_string(IP), 5000);
//   tcp::socket socket(service);
//   std::cout << "[Client] Connecting to server..." << std::endl;
//   socket.connect(endpoint);
//   std::cout << "[Client] Connection successful" << std::endl;

//   socket.async_read_some(boost::asio::buffer(buff), ReadHandler);
//   service.run();
// }

// int main(int argc, char **argv) {
//   if(argc == 1){
//     std::cerr << "Usage: async_tcp_echo_server <port>" << std::endl;
//     return -1;
//   }

//   std::cout << "starting client" << std::endl;

//   Client(argv[1]);

//   return 0;
// }
//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "Update.hpp"

using boost::asio::ip::tcp;

typedef std::deque<Update> chat_message_queue;

class chat_client
{
public:
  chat_client(boost::asio::io_context& io_context,
              const tcp::resolver::results_type& endpoints)
      : io_context_(io_context),
        socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const Update& msg)
  {
    boost::asio::post(io_context_,
                      [this, msg]()
                      {
                        bool write_in_progress = !write_msgs_.empty();
                        write_msgs_.push_back(msg);
                        if (!write_in_progress)
                        {
                          do_write();
                        }
                      });
  }

  void close()
  {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    boost::asio::async_connect(socket_, endpoints,
                               [this](boost::system::error_code ec, tcp::endpoint)
                               {
                                 if (!ec)
                                 {
                                   do_read_header();
                                 }
                               });
  }

  void do_read_header()
  {
    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_.data(), Update::HeaderLength),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                              if (!ec && read_msg_.MakeHeader())
                              {
                                do_read_body();
                              }
                              else
                              {
                                socket_.close();
                              }
                            });
  }

  void do_read_body()
  {
    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_.Body(), read_msg_.RetBodyLength()),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                              if (!ec)
                              {
                                std::cout.write(read_msg_.Body(), read_msg_.RetBodyLength());
                                std::cout << "\n";
                                do_read_header();
                              }
                              else
                              {
                                socket_.close();
                              }
                            });
  }

  void do_write()
  {
    boost::asio::async_write(socket_,
                             boost::asio::buffer(write_msgs_.front().data(),
                                                 write_msgs_.front().length()),
                             [this](boost::system::error_code ec, std::size_t /*length*/)
                             {
                               if (!ec)
                               {
                                 write_msgs_.pop_front();
                                 if (!write_msgs_.empty())
                                 {
                                   do_write();
                                 }
                               }
                               else
                               {
                                 socket_.close();
                               }
                             });
  }

private:
  boost::asio::io_context& io_context_;
  tcp::socket socket_;
  Update read_msg_;
  chat_message_queue write_msgs_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });

    char line[Update::MaxBodyLength + 1];
    while (std::cin.getline(line, Update::MaxBodyLength + 1))
    {
      Update msg;
      msg.MkBodyLength(std::strlen(line));
      std::memcpy(msg.Body(), line, msg.RetBodyLength());
      msg.EncodeHeader();
      c.write(msg);
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
#endif //TEXAS_HOLD_EM_CLIENT_CPP

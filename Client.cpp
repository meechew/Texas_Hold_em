// Created by CBunt on 28 May 2020.
//

#ifndef TEXAS_HOLD_EM_CLIENT_CPP
#define TEXAS_HOLD_EM_CLIENT_CPP
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "Update.hpp"
#include "Package.hpp"

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
                            boost::asio::buffer(read_msg_.RetData(), Update::HeaderLength),
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
                             boost::asio::buffer(write_msgs_.front().RetData(),
                                                 write_msgs_.front().Length()),
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
    if (argc != 4)
    {
      std::cerr << "Usage: TexasClient <player-name> <host> <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[2], argv[3]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });

    ClientPackage lfPack(true, false, false, argv[1]);
    ClientPackage sPack(false, true, false, argv[1]);
    Update msg;
    std::stringstream StringBuff;
    while (true) {
      StringBuff << lfPack;

      msg.MkBodyLength(std::strlen(StringBuff.str().c_str())+1);
      std::memcpy(msg.Body(), StringBuff.str().c_str(), msg.RetBodyLength());
      msg.EncodeHeader();
      std::cerr << "<-SENDING LF-> "; 
      c.write(msg);

      //StringBuff << sPack;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      
      msg.MkBodyLength(std::strlen(StringBuff.str().c_str())+1);
      std::memcpy(msg.Body(), StringBuff.str().c_str(), msg.RetBodyLength());
      msg.EncodeHeader();
      //std::cerr << "<-SENDING STEP-> ";

      //c.write(msg);
      
      std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
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

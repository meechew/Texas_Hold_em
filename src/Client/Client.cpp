// Created by CBunt on 28 May 2020.
//

#ifndef TEXAS_HOLD_EM_CLIENT_CPP
#define TEXAS_HOLD_EM_CLIENT_CPP
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../NetworkAssets/Message.hpp"
#include "../NetworkAssets/Package.hpp"

using boost::asio::ip::tcp;
using namespace boost::uuids;

typedef std::deque<Message> chat_message_queue;

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

  void write(const Message& msg)
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
                            boost::asio::buffer(read_msg_.header(), Message::HEADER_LENGTH),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                              if (!ec && read_msg_.decode_header())
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
                            boost::asio::buffer(read_msg_.body(), read_msg_.get_body_length()),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                              if (!ec)
                              {
                                std::cout.write(read_msg_.body(), read_msg_.get_body_length());
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
                             boost::asio::buffer(write_msgs_.front().header(),
                                                 write_msgs_.front().length()),
                             [this](boost::system::error_code ec, std::size_t /*length*/)
                             {
                               if (!ec)
                               {
                                 std::cerr << write_msgs_.front().body() << std::endl << "message sent";
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
  Message read_msg_;
  chat_message_queue write_msgs_;
};

inline void run_client(const char* name, const char* host, const char* port)
{
    uuid _uuid = random_generator()();

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, port);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });

    ClientPackage lfPack(1, false, false, name, _uuid);
    ClientPackage sPack(0, true, false, name, _uuid);
    Message msg;
    std::stringstream StringBuff;
    while (lfPack.heart_beat_ < 10) {
      StringBuff.str("");
      StringBuff.clear();
      StringBuff << lfPack;

      msg.allocate_body(std::strlen(StringBuff.str().c_str()));
      std::memcpy(msg.body(), StringBuff.str().c_str(), msg.get_body_length());
      msg.encode_header();
      std::cerr << "sending heartbeat: " << lfPack.heart_beat_ << std::endl;
      c.write(msg);

      //StringBuff << sPack;
      boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
      
      msg.allocate_body(std::strlen(StringBuff.str().c_str()));
      std::memcpy(msg.body(), StringBuff.str().c_str(), msg.get_body_length());
      msg.encode_header();
      //std::cerr << "<-SENDING STEP-> ";

      //c.write(msg);
      
      boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));
      ++lfPack.heart_beat_;
    }
    exit(0);
    c.close();
    t.join();
}
#endif //TEXAS_HOLD_EM_CLIENT_CPP

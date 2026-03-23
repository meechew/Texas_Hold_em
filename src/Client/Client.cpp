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
#include "../NetworkAssets/NetworkController.hpp"
#include "../NetworkAssets/Package.hpp"

using boost::asio::ip::tcp;
using namespace boost::uuids;

typedef std::deque<Message> chat_message_queue;

inline void run_client(const char* name, const char* host, const char* port)
{
    uuid _uuid = random_generator()();

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, port);
    auto _client = ClientController::create(io_context);
    _client->start();

    ClientPackage lfPack(1, false, false, name, _uuid);
    ClientPackage sPack(0, true, false, name, _uuid);
    Message msg;
    std::stringstream StringBuff;
    while (lfPack.heart_beat_ < 10) {
      StringBuff.str("");
      StringBuff.clear();
      StringBuff << lfPack;

      msg.new_message(StringBuff, _uuid);
      msg.encode_header();
      std::cerr << "sending heartbeat: " << lfPack.heart_beat_ << std::endl;
      _client->write_message(msg);

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
}
#endif //TEXAS_HOLD_EM_CLIENT_CPP

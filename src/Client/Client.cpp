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

    auto _client = ClientController::create(io_context);
    _client->set_host(host);
    _client->set_port(port);
    _client->start();

    boost::thread _io_thread([&io_context]() { io_context.run(); });
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));  // wait for connect

    ClientPackage lfPack(1, false, false, name, _uuid);
    while (lfPack.heart_beat_ < 3) {
      std::stringstream ss;
      ss << lfPack;
      std::string body = ss.str();

      Message msg;
      msg.allocate_body(body.size());
      std::memcpy(msg.body(), body.c_str(), msg.get_body_length());
      std::memcpy(msg.uuid(), _uuid.data, 16);
      msg.encode_header();

      std::cerr << "sending heartbeat: " << lfPack.heart_beat_ << std::endl;
      // Post to io_context thread to avoid data race on message_queue_
      boost::asio::post(io_context, [_client, msg]() { _client->write_message(msg); });

      boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
      ++lfPack.heart_beat_;
    }

    io_context.stop();
    _io_thread.join();
}
#endif //TEXAS_HOLD_EM_CLIENT_CPP

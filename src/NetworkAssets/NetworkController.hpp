// Created by CBunt on 20 May 2020.
//
// Documentation for this controller can be found at:
// https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio.html

#ifndef TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#define TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP
#include <cstdlib>
#include <utility>
#include <boost/asio.hpp>
#include <boost/container/deque.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind/bind.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/bimap.hpp>
#include "Package.hpp"
#include "Message.hpp"
#include "../Player/Player.hpp"
#include "../Games/Game.hpp"

using boost::asio::ip::tcp;
using namespace boost::uuids;

class Session;

class NetworkController : public boost::enable_shared_from_this<NetworkController>
{
public:
    typedef boost::shared_ptr<NetworkController> ptr;
    typedef boost::bimap<uuid, boost::shared_ptr<Session>> session_map;

    NetworkController(boost::asio::io_context& c) : socket_(c) {}

    virtual ~NetworkController() = default;

    virtual void start() = 0;
    void read_message();
    void write_message(const Message& msg);
protected:

    void handle_read_header(const boost::system::error_code& ec);
    void handle_read_body(const boost::system::error_code& ec);
    void handle_write(const boost::system::error_code& ec);

    tcp::socket socket_;
    Message current_msg_;
    deque<Message> message_queue_;
    session_map session_bimap_;
};

class ServerController : public NetworkController
{
public:
    typedef boost::shared_ptr<ServerController> ptr;

    static ptr create(boost::asio::io_context& c)
    {
        return ptr(new ServerController(c));
    }

    void set_port(std::string p);
    void start() override;

private:
    std::string port;
    ServerController(boost::asio::io_context& c) :
        NetworkController(c),
         acceptor_(socket_.get_executor())
        {}
    tcp::acceptor acceptor_;
    void accept_connections();
};

class ClientController : public NetworkController
{
public:
    typedef boost::shared_ptr<ClientController> ptr;

    static ptr create(boost::asio::io_context& c)
    {
        return ptr(new ClientController(c));
    }

    void set_host(std::string h);
    void set_port(std::string p);
    void start() override;

private:
    std::string host;
    std::string port;
    void connect_to_server(const tcp::resolver::results_type& e);
    ClientController(boost::asio::io_context& c) : NetworkController(c) {}
};

#endif //TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP

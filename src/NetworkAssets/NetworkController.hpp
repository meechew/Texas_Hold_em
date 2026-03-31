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
#include "Package.hpp"
#include "Message.hpp"
#include "../Player/Player.hpp"
#include "../Games/Game.hpp"

using boost::asio::ip::tcp;
using namespace boost::uuids;

class GameTable;

class NetworkController : public boost::enable_shared_from_this<NetworkController>
{
public:
    typedef boost::shared_ptr<NetworkController> ptr;

    NetworkController(boost::asio::io_context& c) : io_context_(c), socket_(c) {}

    virtual ~NetworkController() = default;

    virtual void start() = 0;
    void read_message();
    void write_message(const Message& msg);

protected:
    virtual void on_message_received(const Message& msg) {}

    void handle_read_header(const boost::system::error_code& ec);
    void handle_read_body(const boost::system::error_code& ec);
    void handle_write(const boost::system::error_code& ec);
    void do_write();

    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    Message current_msg_;
    boost::container::deque<Message> message_queue_;
};

class ServerController : public NetworkController
{
public:
    typedef boost::shared_ptr<ServerController> ptr;

    static ptr create(boost::asio::io_context& c, GameTable* gt, int port)
    {
        return ptr(new ServerController(c, gt, std::to_string(port), false));
    }

    // Overload without GameTable (testing)
    static ptr create(boost::asio::io_context& c, int port)
    {
        return ptr(new ServerController(c, nullptr, std::to_string(port), false));
    }

    void start() override;

protected:
    void on_message_received(const Message& msg) override;

private:
    std::string port_;
    bool is_session_;
    tcp::acceptor acceptor_;
    GameTable* game_table_;

    // Listener constructor
    ServerController(boost::asio::io_context& c, GameTable* gt, std::string port, bool session)
        : NetworkController(c), port_(std::move(port)), is_session_(session),
          acceptor_(c), game_table_(gt) {}

    // Creates a session (accepted connection, no acceptor)
    static ptr create_session(boost::asio::io_context& c, GameTable* gt)
    {
        return ptr(new ServerController(c, gt, "", true));
    }

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
    std::string host_;
    std::string port_;
    void connect_to_server(const tcp::resolver::results_type& endpoints);
    ClientController(boost::asio::io_context& c) : NetworkController(c) {}
};

#endif //TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP

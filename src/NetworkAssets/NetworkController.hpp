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

typedef boost::shared_ptr<Player> PlayerPtr;
typedef boost::container::deque<ClientPackage> UpdateQueue;

class Seat
{
protected:
    UpdateQueue incoming_queue_;
public:
    virtual ~Seat() = default;
    // virtual void signal(const Message& msg) = 0;
    virtual void reset_timer() = 0;
    virtual void count_timer() = 0;
};

typedef boost::shared_ptr<Seat> SeatPtr;



struct Finals
{
    PlayerPtr player_;
    Cards final_hand_;
    ScoreBoard final_score_;
};

typedef boost::array<Finals, 5> PlayerFinals;
typedef boost::bimap<SeatPtr, PlayerPtr> SocketSeatLink;
typedef SocketSeatLink::value_type SocketSeatPair;

class GameTable;

class Session :
    public Seat,
    public boost::enable_shared_from_this<Session>
{
public:
    explicit Session(tcp::socket skt)
        : socket_(std::move(skt))
    {
    }

    void start();

    void join() { joined_ = true; }
    void reset_timer() override { timer_ = 0; }
    void count_timer() override { ++timer_; }
    void process_messages();

private:
    bool joined_ = false;
    int timer_ = 0;
    tcp::socket socket_;
    PlayerPtr seated_player_pointer_ = nullptr;
    void do_read();
    void do_write(std::size_t s);

    Message incoming_message_;
    char read_buffer_[Message::HEADER_LENGTH + Message::MAX_BODY_LENGTH] = "";
};

class NetworkController :
    public boost::enable_shared_from_this<NetworkController>
{
public:
    NetworkController(
        boost::asio::io_context& c,
        const tcp::endpoint& e
    ) :
        acceptor_(c, e), context_(c), socket_(c)
    {
        start_accept();
    }
    typedef boost::shared_ptr<NetworkController> Pointer;
    typedef boost::shared_ptr<Session> SessionPtr;
    void start_accept();

    static Pointer create(boost::asio::io_context& c, tcp::endpoint& e)
    {
        return boost::make_shared<NetworkController>(c, e);
    }
    tcp::socket& socket() { return socket_; }
    SessionPtr session() { return session_; }

private:
    tcp::acceptor acceptor_;
    boost::asio::io_context& context_;
    tcp::socket socket_;
    SessionPtr session_ = nullptr;
    SocketSeatLink seat_;
    void handle_accept(const boost::system::error_code& ec);

    Message incoming_msg_;
    char read_buffer_[Message::HEADER_LENGTH + Message::MAX_BODY_LENGTH] = "";
};


#endif //TEXAS_HOLD_EM_NETWORKCONTROLLER_HPP

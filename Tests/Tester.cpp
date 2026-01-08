// Created by CBunt on 16 May 2020.
//

#define BOOST_TEST_MODULE Main_Test_Module
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "include/faker-cxx/esport.h"
#include "faker-cxx/number.h"
// #include "Deck/Deck.hpp"
// #include "Deck/FaceValues.hpp"
#include <boost/interprocess/streams/bufferstream.hpp>

#include "../src/NetworkAssets/NetworkController.hpp"
#include "../src/NetworkAssets/Package.hpp"
#include "../src/Client/Client.cpp"

void start_client(const char* name, const char* host, const char* port)
{
    run_client(name, host, port);
}

BOOST_AUTO_TEST_CASE(package_encode_decode_test)
{
    BOOST_TEST_MESSAGE("   ---Build Client Package---");
    string _test_name = faker::esport::player().data();
    int _test_heart_beat = faker::number::integer(0, 9);
    boost::uuids::uuid _uuid = boost::uuids::random_generator()();

    ClientPackage _client_package(_test_heart_beat, false, false, _test_name, _uuid);
    Message _message;
    std::stringstream _source_buffer;

    _source_buffer.str("");
    _source_buffer.clear();
    _source_buffer << _client_package;

    BOOST_TEST_MESSAGE("   ---Encode Message---");
    size_t _body_length = std::strlen(_source_buffer.str().c_str());
    _message.allocate_body(_body_length);
    std::memcpy(_message.body(), _source_buffer.str().c_str(), _message.get_body_length());
    _message.encode_header();
    _message.decode_header();
    BOOST_TEST(_message.get_body_length() == _body_length + 1);

    boost::interprocess::bufferstream _destination_buffer(_message.body(), _message.get_body_length());
    ClientPackage _destination_package;
    _destination_buffer >> _destination_package;


    BOOST_TEST_MESSAGE("   ---Compare Received Package---");
    BOOST_TEST(_test_name == _destination_package.name_);
    BOOST_TEST(_test_heart_beat == _destination_package.heart_beat_);
    BOOST_TEST(_client_package.leave_ == _destination_package.leave_);
    BOOST_TEST(_client_package.next_step_ == _destination_package.next_step_);
    BOOST_TEST(_uuid == _destination_package.uuid_);

    Player _player(_destination_package.name_, _destination_package.uuid_);
    BOOST_TEST(_player.who() == _test_name);
    BOOST_TEST(_player.get_uuid() == _uuid);

    // ToDo: Test server packages.
    ServerPackage _server_package;
}

BOOST_AUTO_TEST_CASE(deck_functions_test)
{
    // ToDo: Think about future tests
    BOOST_TEST_MESSAGE("Build deck functions");
    Deck _deck;
    Hand _hand;
    Cards _common_cards(_deck.deal(5));

    _hand = _deck.deal_hand();
}


BOOST_AUTO_TEST_CASE(network_functions_test)
{
    BOOST_TEST_MESSAGE("   ---Network Functions---");

    boost::asio::io_context _server_context;

    tcp::endpoint _server_endpoint(tcp::v4(), 5000);
    auto _network_controller = NetworkController::create(_server_context, _server_endpoint);

    boost::thread _context_thread([&_server_context]() { _server_context.run(); });
    boost::thread _client_thread(start_client, "TestPlayer", "127.0.0.1", "5000");


    while (!_server_context.stopped())
    {
        sleep(1);
        _network_controller->session()->process_messages();
    };

    // // boost::asio::io_context _client_context;
    // // tcp::resolver _resolver(_client_context);
    // // auto _client_endpoint = _resolver.resolve("127.0.0.1", "5000");
    // //
    // // chat_client _client(_client_context, _client_endpoint);
    // // _client.write(_message);
    // boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));
    // _table.step();
}

/*
int main()
{
    Deck T_Deck;
    Hand T_Hand;
    Cards T_Common;
    T_Hand.first = T_Deck.deal(1).front();
    T_Hand.second = T_Deck.deal(1).front();

    T_Common = T_Deck.deal(5);

    std::cout << "###COMMON CARDS###" << std::endl;
    for (auto k : T_Common)
    {
        std::cout << "-->" << Rank[k.rank] << SuitChar[k.suit] << std::endl;
    }
    Player Player1(test_name);
    Player1.new_hand(T_Deck.deal(2));
    T_Hand = Player1.call();
    std::cout << "Player1 Name -->" << Player1.who() << std::endl <<
        "Player1 Hand -->" << Rank[T_Hand.first.rank] <<
        SuitChar[T_Hand.first.suit] << ',' << Rank[T_Hand.second.rank] <<
        SuitChar[T_Hand.second.suit] << std::endl;

    Player1.fold();
    T_Hand = Player1.call();
    std::cout << "Player1 Fold -->" << Rank[T_Hand.first.rank] << SuitChar[T_Hand.first.suit] <<
        ',' << Rank[T_Hand.second.rank] << SuitChar[T_Hand.second.suit];

    Player1.new_hand(T_Deck.deal(2));
    ServerPackage Pack(false, false, false, std::move(Player1.who()), "Winner!!", Player1.call(), T_Common);

    Pack.heart_beat_ = false;
    Pack.winner_notice_ = false;
    Pack.name_ = std::move(Player1.who());
    Pack.winner_ = "Winner!!";
    Pack.hand_ = Player1.call();
    for (int k = 0; k < 5; ++k)
    {
        Pack.table_[k] = T_Common[k];
    }

    std::cout << "\nPackaged: \n" << Pack << std::endl;

    boost::asio::io_context Context;
    tcp::endpoint Endpoint(tcp::v4(), 5000);
    boost::thread Client(start_client);
    // Client.detach();

    Table T_Table(Context, Endpoint);
    T_Table.step();
    T_Table.step();
    T_Table.step();
    T_Table.step();
    std::cout << "\nPackaged: \n" << *T_Table.make_package(0, false, false, false) << '\n';
}
*/

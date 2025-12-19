// Created by CBunt on 16 May 2020.
//

#include "Deck/Deck.hpp"
#include "Deck/FaceValues.hpp"
#include "include/faker-cxx/esport.h"
#include "NetworkAssets/NetworkController.hpp"
#include "NetworkAssets/Package.hpp"

void start_client()
{
    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    std::system("./TexasClient ClientTest 127.0.0.1 5000");
}

int main()
{
    string test_name = faker::esport::player().data();
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

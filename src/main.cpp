#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "GameTable/GameTable.hpp"
#include "NetworkAssets/NetworkController.hpp"

int main()
{
    try
    {
        boost::asio::io_context context;

        GameTable table;

        auto server = ServerController::create(context, &table, 5000);
        server->start();

        boost::thread heartbeat([&table]() { table.start_heart_beat(); });

        std::cout << "Server listening on port 5000\n";
        context.run();

        heartbeat.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
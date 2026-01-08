// Created by meechew on 1/3/26.
//

#ifndef START_CLIENT_CPP
#define START_CLIENT_CPP

#include "Client.cpp"

int main(int argc, char* argv[])
{

    try
    {
        if (argc != 4)
        {
          std::cerr << "Usage: TexasClient <player-name> <host> <port>\n";
          return 1;
        }
        char* name = argv[1];
        char* host = argv[2];
        char* port = argv[3];

        run_client(name, host, port);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    exit(0);
}
#endif //START_CLIENT_CPP

#include <iostream>
#include "NetworkController.hpp"

int main() {
  try
  {
    boost::asio::io_context Context;
    tcp::endpoint Endpoint(tcp::v4(), 5000);

    Table ServerGame(Context, Endpoint);


  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

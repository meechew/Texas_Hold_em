#include <iostream>
#include "NetworkController.hpp"

int main() {
  try
  {


    boost::asio::io_context Context;
    tcp::endpoint endpoint(tcp::v4(), 5000);

    NetworkController Server(Context, endpoint);

    Context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

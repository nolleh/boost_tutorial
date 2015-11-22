#include <iostream>
#include <string>
#include "boost/asio.hpp"

namespace nolleh {
namespace intro_socket{
using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  std::time_t now = time(0);
  return ctime(&now);
}
void server_run()
{
  try
  {
	  boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));
    io_service.run();
    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      std::string message = make_daytime_string();
      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

}
}
#include <iostream>
#include "boost/asio.hpp"
#include "boost/array.hpp"

namespace nolleh {
namespace intro_socket {
using boost::asio::ip::tcp;

void client_run(std::string name){

  try {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(
      boost::asio::ip::tcp::resolver::query("127.0.0.1", name));
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);
    io_service.run();

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);
      if (error == boost::asio::error::eof)
        break;
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

}
}
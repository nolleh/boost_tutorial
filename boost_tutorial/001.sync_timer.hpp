// blocking wait - timer
#include <iostream>

#include "boost/asio/io_service.hpp"
#include "boost/asio/deadline_timer.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#pragma comment(lib, "libboost_system-vc140-mt-gd-1_59.lib")

namespace nolleh {
namespace tutorial {

class SyncTimer {
public:
  void wait_print()
  {
    boost::asio::io_service io;
    boost::asio::deadline_timer t(
      io, boost::posix_time::seconds(5));
    t.wait(); // expired_timer
    std::cout << "Hello, world!\n";
  }
};

}
}
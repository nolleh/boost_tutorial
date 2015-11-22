#include <iostream>

#include "boost/asio/io_service.hpp"
#include "boost/asio/deadline_timer.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#pragma comment(lib, "libboost_system-vc140-mt-gd-1_59.lib")

namespace nolleh {
namespace tutorial {

class PeriodTimer{
public:
  void wait_print()
  {
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
    t.async_wait(
        std::bind(&PeriodTimer::print, this, std::placeholders::_1, &t, 0));
    io.run();
  }
private:
  void print(const boost::system::error_code& /* e */,
    boost::asio::deadline_timer* timer, int count)
  {
    if (count >= 5)
      return;

    std::cout << "Hello, period Timer world!, count : " << count << std::endl;
    timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));
    timer->async_wait(
      std::bind(&PeriodTimer::print, this, std::placeholders::_1, timer, ++count));
  }
};

}
}
// non blocking wait 0 timer
#include <iostream>

#include "boost/asio/io_service.hpp"
#include "boost/asio/deadline_timer.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#pragma comment(lib, "libboost_system-vc140-mt-gd-1_59.lib")

namespace nolleh {
namespace tutorial {

class AsyncTimer {
public:
  void wait_print()
  {
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
    t.async_wait(std::bind(&AsyncTimer::print, this, std::placeholders::_1));

    // 이 함수를 호출한 스레드에서 핸들러가 호출되는 것을 보장한다.
    // 즉, 이 함수를 호출하지 않으면 콜백이 호출되지 않는다. 
    // 이 함수는 'work' 가 있는 동안 리턴되지 않으며,
    // 여기서는 async_wait 이 work 라고 할 수 있다. 
    // 이 work 가 없으면 호출 즉시 리턴될 것이다.
    io.run();

    // 이 io_service run 을 호출한 스레드가
    // main 스레드이므로 5초 대기후 리턴한다.
  }
private:
  void print(const boost::system::error_code& /* e */)
  {
    std::cout << "Hello, async world!\n";
  }
};
}
}
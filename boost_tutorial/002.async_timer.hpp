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

    // �� �Լ��� ȣ���� �����忡�� �ڵ鷯�� ȣ��Ǵ� ���� �����Ѵ�.
    // ��, �� �Լ��� ȣ������ ������ �ݹ��� ȣ����� �ʴ´�. 
    // �� �Լ��� 'work' �� �ִ� ���� ���ϵ��� ������,
    // ���⼭�� async_wait �� work ��� �� �� �ִ�. 
    // �� work �� ������ ȣ�� ��� ���ϵ� ���̴�.
    io.run();

    // �� io_service run �� ȣ���� �����尡
    // main �������̹Ƿ� 5�� ����� �����Ѵ�.
  }
private:
  void print(const boost::system::error_code& /* e */)
  {
    std::cout << "Hello, async world!\n";
  }
};
}
}
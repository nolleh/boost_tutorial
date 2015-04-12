#include "005.strand.hpp"
#include <thread>
#include "boost/asio/io_service.hpp"
#include "boost/bind.hpp"
int main()
{
  boost::asio::io_service io_service;
  nolleh::tutorial::Printer printer(io_service);
  // 백그라운드 스레드 t 와 메인스레드에서 run 을 호출했다.
  std::thread t(std::bind(
    /* io_service::run() 오버로딩 함수를 deduce 하기 위해 시그니쳐 명시 */
    // http://stackoverflow.com/questions/9048119/why-cant-stdbind-and-boostbind-be-used-interchangeably-in-this-boost-asio-t
    static_cast<size_t (boost::asio::io_service::* )()>(
    &boost::asio::io_service::run), &io_service));
  io_service.run();
  t.join();
  return 0;
}
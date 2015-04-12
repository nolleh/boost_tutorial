#include "005.strand.hpp"
#include <thread>
#include "boost/asio/io_service.hpp"
#include "boost/bind.hpp"
int main()
{
  boost::asio::io_service io_service;
  nolleh::tutorial::Printer printer(io_service);
  // ��׶��� ������ t �� ���ν����忡�� run �� ȣ���ߴ�.
  std::thread t(std::bind(
    /* io_service::run() �����ε� �Լ��� deduce �ϱ� ���� �ñ״��� ��� */
    // http://stackoverflow.com/questions/9048119/why-cant-stdbind-and-boostbind-be-used-interchangeably-in-this-boost-asio-t
    static_cast<size_t (boost::asio::io_service::* )()>(
    &boost::asio::io_service::run), &io_service));
  io_service.run();
  t.join();
  return 0;
}
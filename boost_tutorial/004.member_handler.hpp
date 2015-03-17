#include <iostream>
#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace nolleh {
namespace tutorial {
class MemberHandler {
public:
  void wait_print()
  {
    boost::asio::io_service io_service;
    Printer printer(io_service);
    io_service.run();
  }
private:
  class Printer
  {
  public:
    Printer(boost::asio::io_service& io) :
      timer_(io, boost::posix_time::seconds(1)), count_(0)
    {
      // 모든 멤버 함수는 this 가 필요하다.
      timer_.async_wait(std::bind(&Printer::print, this));
    }
    ~Printer()
    {
      std::cout << "Final count is " << count_ << std::endl;
    }
  private:
    void print()
    {
      if (count_ < 5)
      {
        std::cout << count_ << std::endl;
        ++count_;
        timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
        timer_.async_wait(std::bind(&Printer::print, this));
      }
    }

    boost::asio::deadline_timer timer_;
    int count_;
  };
};

}
}
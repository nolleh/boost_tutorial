#include <iostream>
#include "boost/asio/io_service.hpp"
#include "boost/asio/strand.hpp"
#include "boost/asio/deadline_timer.hpp"

namespace nolleh {
namespace tutorial {
class Printer {
public:
  Printer(boost::asio::io_service& io) :
    strand_(io),
    timer1_(io, boost::posix_time::seconds(1)),
    timer2_(io, boost::posix_time::seconds(1)),
    count_(0)
  {
    timer1_.async_wait(strand_.wrap(std::bind(&Printer::print1, this)));
    timer2_.async_wait(strand_.wrap(std::bind(&Printer::print2, this)));
  }

  ~Printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }
private:

  void print1()
  {
    if (count_ < 10)
    {
      std::cout << "Timer 1:" << count_ << std::endl;
      ++count_;

      timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
      timer1_.async_wait(strand_.wrap(std::bind(&Printer::print1, this)));
    }
  }

  void print2()
  {
    if (count_ < 10)
    {
      std::cout << "Timer 2:" << count_ << std::endl;
      ++count_;

      timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
      timer2_.async_wait(strand_.wrap(std::bind(&Printer::print2, this)));
    }
  }

  boost::asio::io_service::strand strand_;
  boost::asio::deadline_timer timer1_;
  boost::asio::deadline_timer timer2_;
  int count_;
};
}
}

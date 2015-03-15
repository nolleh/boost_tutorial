#include "002.async_timer.hpp"
#include "003.binding_to_handler.hpp"

int main()
{
  //nolleh::tutorial::AsyncTimer async_timer;
  //async_timer.wait_print();
  nolleh::tutorial::PeriodTimer period_timer;
  period_timer.wait_print();
  return 0;
}
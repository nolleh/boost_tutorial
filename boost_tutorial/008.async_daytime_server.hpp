#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include "boost/shared_ptr.hpp"
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/enable_shared_from_this.hpp"

//* enable_shared_from_this 를 활용하여 이 객체에 대한 연산이 수행되는 경우 소멸되지 않도록 보장한다.
//* 클라이언트로 전달하는 데이터가 소멸되지 않게 하기 위해, 클래스의 멤버로 데이터를 보전한다.
//* 전송 데이터가 분리되지 않게 하기 위해 async_write() 함수를 활용
//* 사용하지 않는 place_holder 를 제거하여 핸들러 인자의 시그니쳐를 단순히 가져 갈 수도 있다(async_write)
class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
public:
  typedef boost::shared_ptr<TCPConnection> pointer;
  static pointer create(boost::asio::io_service& io)
  {
    return pointer(new TCPConnection(io));
  }

  boost::asio::ip::tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
      boost::bind(&TCPConnection::handle_write, shared_from_this()));
  }

private:
  TCPConnection(boost::asio::io_service& io) : socket_(io)
  {
  }

  void handle_write()
  {
    std::cout << message_ << ", write_done" << std::endl;
  }

  std::string make_daytime_string()
  {
    std::time_t now = time(0);
    return ctime(&now);
  }

  boost::asio::ip::tcp::socket socket_;
  std::string message_;
};

//* boost::asio::ip::tcp::begin_accept 를 활용하여 클라이언트의 연결을 받고, tcp_connection 객체를 생성한다.
//* connection 의 start() 함수를 호출한다.
//* 완료 된 후, 다음 연결을 받기 위해 다시 accept 를 시작한다.
class TCPServer
{
public:
  TCPServer(boost::asio::io_service& io) :
    acceptor_(io, boost::asio::ip::tcp::endpoint(
      boost::asio::ip::tcp::v4(), 13))
  {
    start_accept();
  }
private:
  void start_accept()
  {
    TCPConnection::pointer new_connection =
      TCPConnection::create(acceptor_.get_io_service());
    // async_accept(socket, ACCEPT_HANDLER);
    acceptor_.async_accept(new_connection->socket(),
      boost::bind(&TCPServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
  }
  void handle_accept(TCPConnection::pointer new_connection,
    const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }
    start_accept();
  }

  boost::asio::ip::tcp::acceptor acceptor_;
};

//* tcp_server 에 io_service 를 전달하여 생성하고, tcp_server 클래스를 initiate 한다.
//* 메인 스레드에서 tcp_server 에서 사용하는 io_service 의 run() 함수 호출
void server_run()
{
  try 
  {
    boost::asio::io_service io_service;
    TCPServer server(io_service);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}


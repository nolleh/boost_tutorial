#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include "boost/shared_ptr.hpp"
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/enable_shared_from_this.hpp"

//* enable_shared_from_this �� Ȱ���Ͽ� �� ��ü�� ���� ������ ����Ǵ� ��� �Ҹ���� �ʵ��� �����Ѵ�.
//* Ŭ���̾�Ʈ�� �����ϴ� �����Ͱ� �Ҹ���� �ʰ� �ϱ� ����, Ŭ������ ����� �����͸� �����Ѵ�.
//* ���� �����Ͱ� �и����� �ʰ� �ϱ� ���� async_write() �Լ��� Ȱ��
//* ������� �ʴ� place_holder �� �����Ͽ� �ڵ鷯 ������ �ñ״��ĸ� �ܼ��� ���� �� ���� �ִ�(async_write)
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

//* boost::asio::ip::tcp::begin_accept �� Ȱ���Ͽ� Ŭ���̾�Ʈ�� ������ �ް�, tcp_connection ��ü�� �����Ѵ�.
//* connection �� start() �Լ��� ȣ���Ѵ�.
//* �Ϸ� �� ��, ���� ������ �ޱ� ���� �ٽ� accept �� �����Ѵ�.
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

//* tcp_server �� io_service �� �����Ͽ� �����ϰ�, tcp_server Ŭ������ initiate �Ѵ�.
//* ���� �����忡�� tcp_server ���� ����ϴ� io_service �� run() �Լ� ȣ��
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


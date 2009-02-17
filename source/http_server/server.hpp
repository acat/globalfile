//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_SERVER_HPP
#define HTTP_SERVER3_SERVER_HPP

#include "connection.hpp"
#include "request_handler.hpp"

namespace http {
namespace server3 {

/// The top-level class of the HTTP server.
class server
  : private boost::noncopyable
{
public:
  /// Construct the server to listen on the specified TCP address and port, and
  /// serve up files from the given directory.
  explicit server(boost::asio::io_service& io_service, const std::string& address, const std::string& port);

private:
  /// Handle completion of an asynchronous accept operation.
  void handle_accept(const boost::system::error_code& e);

  boost::asio::io_service& io_service_;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The next connection to be accepted.
  ConnectionPtr new_connection_;

  /// The handler for all incoming requests.
  request_handler request_handler_;
};

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_SERVER_HPP

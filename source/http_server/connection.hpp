//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_CONNECTION_HPP
#define HTTP_SERVER3_CONNECTION_HPP

#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"

#ifndef File
class File;
typedef boost::shared_ptr<File> FilePtr;
#endif

namespace http {
namespace server3 {

/// Represents a single connection from a client.
class Connection
  : public boost::enable_shared_from_this<Connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_service.
  explicit Connection(boost::asio::io_service& io_service,
      request_handler& handler);

  ~Connection();

  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void start();

  void handle_read_file(const byte* buffer, size_t read);

private:
  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,
      std::size_t bytes_transferred);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e);

  /// Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_service::strand strand_;

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The handler used to process the incoming request.
  request_handler& request_handler_;

  /// Buffer for incoming data.
  boost::array<byte, 1024> buffer_;

  /// The incoming request.
  request request_;

  /// The parser for the incoming request.
  request_parser request_parser_;

  /// The reply to be sent back to the client.
  reply reply_;

  FilePtr file_;
  std::size_t pos_;
};

} // namespace server3
} // namespace http

typedef boost::shared_ptr<http::server3::Connection> ConnectionPtr;

#endif // HTTP_SERVER3_CONNECTION_HPP

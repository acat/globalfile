#ifndef _H_GLOBAL_FILE
#define _H_GLOBAL_FILE

#include "http_server/server.hpp"

/// The io_service used to perform asynchronous operations.
extern boost::shared_ptr<boost::asio::io_service> io_service;

extern boost::shared_ptr<http::server3::server> http_server;

//extern BlockManagerPtr block_manager;

//extern NodeManagerPtr node_manager;

//extern LogPtr logger;

#endif

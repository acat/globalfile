//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "stdafx.h"

#include "request_handler.hpp"
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "../file.h"

namespace http {
namespace server3 {

request_handler::request_handler()
{
}

void request_handler::handle_request(const request& req, reply& rep, FilePtr& file_ptr)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(),
               (int(*)(int)) std::tolower);
  }

  // Open the file to send back.
  file_ptr = File::create("http://localhost:4490" + request_path);
  if (!file_ptr)
  {
    rep = reply::stock_reply(reply::not_found);
    return;
  }

  file_ptr->open();
  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  rep.headers.resize(4);
  rep.headers[0].name = "Accept-Ranges";
  rep.headers[0].value = "bytes";
  rep.headers[1].name = "Content-Length";
  rep.headers[1].value = boost::lexical_cast<std::string>(file_ptr->size());
  rep.headers[2].name = "Content-Type";
  rep.headers[2].value = mime_types::extension_to_type(extension);
  rep.headers[3].name = "Last-Modified";
  rep.headers[3].value = "Mon, 12 May 2008 06:28:00 GMT";
  //rep.headers[3].name = "Content-Range";
  //rep.headers[3].value = "bytes 0-3230748/3230749";
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server3
} // namespace http

#include "stdafx.h"

#include "log.h"

Log::Log(const char* filename)
: os(filename)
{
}

void Log::log(NodePtr node, char * format, ...)
{
    boost::mutex::scoped_lock lock(mutex_);

    char buffer[1024];
    va_list args;
    va_start (args, format);
    vsprintf (buffer, format, args);
    os << node->id().to_hex_string().substr(0, 8) << "..." << "[" << hex  << (unsigned int)node->ip() << ":" << dec << (unsigned short)node->port() << "] " << buffer;
    os.flush();
    va_end (args);
}

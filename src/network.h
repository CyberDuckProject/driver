#ifndef NETWORK_H
#define NETWORK_H

#include "common_headers.h"
#include <boost/asio.hpp>

class signal_handler
{
public:
    signal_handler(boost::asio::thread_pool& ctx);

private:
    boost::asio::signal_set signal_set;
};

#endif

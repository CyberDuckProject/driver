#ifndef UTL_ASIO_H
#define UTL_ASIO_H

#include <boost/asio.hpp>

using execution_context = boost::asio::thread_pool;
using executor = boost::asio::thread_pool::executor_type;
using strand = boost::asio::strand<boost::asio::thread_pool::executor_type>;

#endif

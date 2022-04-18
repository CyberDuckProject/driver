#ifndef UTL_ASIO_H
#define UTL_ASIO_H

#include <boost/asio.hpp>

using execution_context = boost::asio::thread_pool;
using executor = execution_context::executor_type;
using strand = boost::asio::strand<executor>;

#endif

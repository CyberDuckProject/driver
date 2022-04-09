#include "network.h"

namespace asio = boost::asio;
namespace bs = boost::system;

signal_handler::signal_handler(asio::thread_pool& ctx) : signal_set{ctx, SIGINT, SIGTERM}
{
    signal_set.async_wait([&ctx](const bs::error_code& ec, i32 sig) {
        if (!ec)
        {
            BOOST_LOG_TRIVIAL(debug) << "received signal " << sig;
            ctx.stop();
        }
        else
        {
            throw bs::system_error{ec};
        }
    });
}

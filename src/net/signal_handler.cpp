#include "signal_handler.h"

#include "utl/fundamental_types.h"
#include <boost/log/trivial.hpp>

namespace bs = boost::system;

signal_handler::signal_handler(execution_context& ctx) :
    signal_set{ctx.get_executor(), SIGINT, SIGTERM}
{
    signal_set.async_wait([&ctx](const bs::error_code& ec, i32 sig) {
        if (!ec)
        {
            BOOST_LOG_TRIVIAL(debug) << "received signal " << sig;
            ctx.stop();
        }
    });
}

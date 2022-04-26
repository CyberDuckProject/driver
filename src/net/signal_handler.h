#ifndef NET_SIGNAL_HANDLER_H
#define NET_SIGNAL_HANDLER_H

#include "utl/fundamental_types.h"
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

namespace net {

template<typename Executor>
class signal_handler
{
public:
    using executor_type = Executor;

    explicit signal_handler(const executor_type& ex) : ex{ex}, signal_set{ex, SIGINT, SIGTERM}
    {
        signal_set.async_wait([ex](boost::system::error_code ec, i32 sig) {
            if (!ec)
            {
                BOOST_LOG_TRIVIAL(debug) << "received signal " << sig;
                ex.context().stop();
            }
            else
            {
                throw boost::system::system_error{ec};
            }
        });
    }

    executor_type get_executor()
    {
        return ex;
    }

private:
    executor_type ex;
    boost::asio::signal_set signal_set;
};

} // namespace net

#endif

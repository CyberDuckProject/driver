#ifndef NET_SIGNAL_HANDLER_H
#define NET_SIGNAL_HANDLER_H

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

template<typename ExecutionContext>
class signal_handler
{
public:
    signal_handler(ExecutionContext& ctx) : signal_set{ctx, SIGINT, SIGTERM}
    {
        signal_set.async_wait([&ctx](const boost::system::error_code& ec, i32 sig) {
            if (!ec)
            {
                BOOST_LOG_TRIVIAL(debug) << "received signal " << sig;
                ctx.stop();
            }
            else
            {
                throw boost::system::system_error{ec};
            }
        });
    }

private:
    boost::asio::signal_set signal_set;
};

#endif

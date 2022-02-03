#include "status_loop.h"
#include <boost/log/trivial.hpp>

namespace asio = boost::asio;


void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "initializing I/O";
    io io;

    BOOST_LOG_TRIVIAL(info) << "initializing thread pool";
    asio::thread_pool ctx;
    
    // Handle exit signals
    asio::signal_set signals{ctx, SIGINT, SIGTERM};
    signals.async_wait([&](const boost::system::error_code& ec, i32 sig) {
        if (ec)
        {
            throw boost::system::system_error{ec};
        }
        assert(sig == SIGINT || sig == SIGTERM);
        BOOST_LOG_TRIVIAL(debug) << "received signal " << sig << ", exiting";
        ctx.stop();
    });

    BOOST_LOG_TRIVIAL(info) << "initializing server";
    status_loop status{ctx, io, std::chrono::seconds{1}};

    ctx.join();
}

int main()
{
    try
    {
        guarded_main();
    }
    catch (const std::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#include "net/connection_manager.h"

namespace asio = boost::asio;

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "starting 4 threads";
    asio::thread_pool ctx{4};

    BOOST_LOG_TRIVIAL(info) << "registering signal handler";
    asio::signal_set signal_set{ctx, SIGINT, SIGTERM};
    signal_set.async_wait([&ctx](boost::system::error_code ec, i32 sig) {
        if (ec)
        {
            BOOST_LOG_TRIVIAL(debug) << "received signal " << sig;
            ctx.stop();
        }
        else
        {
            throw boost::system::system_error{ec};
        }
    });

    BOOST_LOG_TRIVIAL(info) << "listening on port 1333";
    net::connection_manager connection_mgr{ctx.get_executor(), 1333};

    ctx.join();
}

i32 main()
{
    try
    {
        guarded_main();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << e.what();
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << "unknown error";
    }

    return EXIT_FAILURE;
}

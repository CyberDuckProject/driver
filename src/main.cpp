#include "net/connection.h"
#include "net/signal_handler.h"

namespace asio = boost::asio;

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "starting 4 threads";
    asio::thread_pool ctx{4};

    BOOST_LOG_TRIVIAL(info) << "registering signal handler";
    net::signal_handler sig_handler{ctx.get_executor()};

    BOOST_LOG_TRIVIAL(info) << "listening on port 1333";
    net::connection conn{ctx.get_executor(), 1333};

    conn.async_accept([](boost::system::error_code ec) {
        BOOST_LOG_TRIVIAL(debug) << "hello";
    });

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

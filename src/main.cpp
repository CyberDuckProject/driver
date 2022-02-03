#include "status_loop.h"
#include <boost/log/trivial.hpp>

namespace asio = boost::asio;

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "initializing I/O";
    io io;

    BOOST_LOG_TRIVIAL(info) << "initializing thread pool";
    asio::thread_pool ctx;

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

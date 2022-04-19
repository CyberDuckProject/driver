#include "io/io_driver.h"
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

namespace asio = boost::asio;

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "initializing I/O driver";
    // io_driver drv;

    BOOST_LOG_TRIVIAL(info) << "starting 4 threads";
    asio::thread_pool ctx{4};

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

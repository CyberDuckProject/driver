#include "io_driver.h"
#include "network.h"

namespace asio = boost::asio;

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "initializing I/O driver";
    // io_driver drv;

    BOOST_LOG_TRIVIAL(info) << "starting 4 threads";
    asio::thread_pool ctx{4};

    BOOST_LOG_TRIVIAL(info) << "registering signal handler";
    signal_handler sighandler{ctx};

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

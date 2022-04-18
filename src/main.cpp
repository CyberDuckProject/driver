#include "core/execution_context.h"
#include "core/io_driver.h"
#include "net/signal_handler.h"
#include <boost/log/trivial.hpp>

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "initializing I/O driver";
    // io_driver drv;

    BOOST_LOG_TRIVIAL(info) << "starting execution context";
    execution_context ctx;

    BOOST_LOG_TRIVIAL(info) << "registering signal handler";
    signal_handler sig_handler{ctx};

    ctx.run();
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

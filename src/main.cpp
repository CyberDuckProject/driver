#include "io_driver.h"

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "initializing I/O driver";
    io_driver drv;
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

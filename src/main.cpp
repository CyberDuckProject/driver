#include "status_loop.h"
#include <boost/log/trivial.hpp>

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "initializing I/O";
    io io;
}

i32 main()
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

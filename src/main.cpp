#include "controller.h"
#include "timed_loop.h"
#include "transmitter.h"

namespace asio = boost::asio;

void guarded_main()
{
    io_driver drv;
    asio::thread_pool ctx{4};

    // Setup manual controls
    controller ctrl{ctx, drv, 1333};
    ctrl.listen();

    // Setup sensor loop
    transmitter tx{ctx, drv};
    timed_loop loop{ctx, std::chrono::seconds{1}, [&]() {
                        if (auto remote{ctrl.remote()}; remote)
                        {
                            tx.send_to(*remote);
                        }
                    }};
    loop.run();

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
        return EXIT_FAILURE;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << "unknown error";
        return EXIT_FAILURE;
    }
}

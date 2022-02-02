#include "io.h"
#include "log.h"

void guarded_main()
{
    LOG(info) << "initializing I/O";
    io io;
}

int main()
{
    try
    {
        guarded_main();
    }
    catch (const std::exception& e)
    {
        LOG(fatal) << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

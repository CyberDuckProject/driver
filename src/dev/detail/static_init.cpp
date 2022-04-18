#include "utl/fundamental_types.h"
#include <pigpio.h>

class static_init
{
public:
    static_init()
    {
        // Disable printing
        u32 internals = gpioCfgGetInternals() | PI_CFG_NOSIGHANDLER;
        gpioCfgSetInternals(internals);

        // Disable unused features
        gpioCfgInterfaces(PI_DISABLE_FIFO_IF | PI_DISABLE_SOCK_IF | PI_DISABLE_ALERT);

        // Initialize pigpio
        gpioInitialise();
    }

    ~static_init()
    {
        gpioTerminate();
    }
};

namespace {

static_init instance;

} // namespace

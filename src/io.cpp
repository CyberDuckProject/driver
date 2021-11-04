#include "io.h"

#include <cassert>
#include <pigpio.h>

namespace io {

void init()
{
	// Disable printing
	u32 internals{gpioCfgGetInternals() | PI_CFG_NOSIGHANDLER};
	gpioCfgSetInternals(internals);

	if (i32 code{gpioInitialise()}; code < 0)
	{
		throw gpio_error{code};
	}
}

void shutdown()
{
	gpioTerminate();
}

} // namespace io

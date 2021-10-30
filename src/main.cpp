#include "gpio_error.h"
#include <pigpio.h>

int main()
{
	i32 result = gpioInitialise();
	if (result < 0)
	{
		throw gpio_error{result};
	}

	gpioTerminate();
}

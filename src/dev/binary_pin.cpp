#include "binary_pin.h"

#include "detail/error_codes.h"
#include <pigpio.h>

binary_pin::binary_pin(u32 broadcom, bool on) : broadcom{broadcom}
{
    pigpio_verify(gpioSetMode(broadcom, PI_OUTPUT));
    set_state(on);
}

binary_pin::~binary_pin()
{
    gpioWrite(broadcom, false);
}

bool binary_pin::state() const
{
    return value;
}

void binary_pin::set_state(bool on)
{
    value = on;
    pigpio_verify(gpioWrite(broadcom, on));
}

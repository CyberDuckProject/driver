#include "esc.h"

#include <cassert>
#include <pigpio.h>

namespace dev {

esc::esc(u32 broadcom, f64 speed) : broadcom{broadcom}
{
    set_speed(speed);
}

esc::~esc()
{
    gpioServo(broadcom, 1000);
}

f64 esc::speed() const
{
    i32 us = gpioGetServoPulsewidth(broadcom);
    return static_cast<f64>(us - 1000) / 1000.0;
}

void esc::set_speed(f64 speed)
{
    assert(speed >= 0.0 && speed <= 1.0);
    u32 us = 1000 + static_cast<u32>(speed * 1000);
    gpioServo(broadcom, us);
}

} // namespace dev

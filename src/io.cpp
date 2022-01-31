#include "io.h"

#include <cassert>

void io::set_left_motor(f32 speed)
{
    assert(speed >= 0.0f && speed <= 1.0f);
    left.set_speed(speed);
}

void io::set_right_motor(f32 speed)
{
    assert(speed >= 0.0f && speed <= 1.0f);
    right.set_speed(speed);
}

void io::set_eyes(bool on)
{
    eyes.set_value(on);
}

void io::set_fan(bool on)
{
    eyes.set_value(on);
}

f32 io::temperature() const
{
    return (adc.read(0) - 0.5f) * 100.0f;
}

f32 io::turbidity() const
{
    // https://forum.arduino.cc/t/getting-the-quantified-ntu-of-turbidity-sensor-sku-sen0189/701459/3
    f32 x = adc.read(1);
    return -2572.2 * x * x + 8700.5 * x - 4352.9;
}

bme280_readout io::weather() const
{
    return bme280.read();
}

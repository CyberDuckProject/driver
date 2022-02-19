#include "io.h"

#include <cassert>

void io::set_left_motor(f32 speed)
{
    assert(speed >= 0.0f && speed <= 1.0f);
    left.set_speed(speed);
}

f32 io::left_motor() const
{
    return left.speed();
}

void io::set_right_motor(f32 speed)
{
    assert(speed >= 0.0f && speed <= 1.0f);
    right.set_speed(speed);
}

f32 io::right_motor() const
{
    return right.speed();
}

void io::set_eyes(bool on)
{
    eyes_pin.set_value(on);
}

bool io::eyes() const
{
    return eyes_pin.value();
}

void io::set_fan(bool on)
{
    fan_pin.set_value(on);
}

bool io::fan() const
{
    return fan_pin.value();
}

void io::set_heartbeat(bool on)
{
    heartbeat_pin.set_value(on);
}

bool io::heartbeat() const
{
    return heartbeat_pin.value();
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

f32 io::battery_voltage() const
{
    f32 scale = 23870.0f / (23870.0f + 66750.0f);
    return adc.read(3) / scale;
}

bme280_readout io::weather() const
{
    return bme280.read();
}

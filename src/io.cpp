#include "io.h"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <unistd.h>

namespace {

io* singleton;

} // namespace

io::io()
{
    assert(!singleton);
    singleton = this;
}

io::~io()
{
    singleton = nullptr;
}

io& io::get()
{
    assert(singleton);
    return *singleton;
}

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

f32 io::temperature()
{
    return (adc.read(0) - 0.5f) * 100.0f;
}

f32 io::turbidity()
{
    f32 scale = 56000.0f / (33000.0f + 56000.0f);
    f32 x = adc.read(1) / scale;
    if (x <= 2.5f)
    {
        BOOST_LOG_TRIVIAL(warning) << "turbidity value out of sensor range";
        return 3000.0f;
    }
    return -1120.4f * x * x + 5742.3f * x - 4352.9;
}

f32 io::dust()
{
    f32 scale = 18000.0f / (33000.0f + 18000.0f);
    f32 sum = 0;
    u32 count = 0;

    for (u32 i = 0; i < 10; ++i)
    {
        // Flash IR and read voltage
        dust_pin.set_value(true);
        usleep(280);
        f32 voltage = adc.read(2) / scale;
        dust_pin.set_value(false);

        // Add to average if measurement is valid
        voltage -= 0.6f;
        if (voltage > 0)
        {
            ++count;
            sum += voltage * 200.0f;
        }
    }

    if (count == 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "measured no dust after 10 iterations";
        return 0.0f;
    }

    return sum / static_cast<f32>(count);
}

f32 io::battery_voltage()
{
    f32 scale = 23.870f / (23.870f + 66.750f);
    return adc.read(3) / scale;
}

peripherals::bme280_readout io::weather()
{
    return bme280.read();
}

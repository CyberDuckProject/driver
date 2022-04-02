#include "io_driver.h"

namespace {

constexpr f64 voltage_divider(f64 r1, f64 r2)
{
    return r2 / (r1 + r2);
}

} // namespace

f64 io_driver::left_motor() const
{
    return left.speed();
}

void io_driver::set_left_motor(f64 speed)
{
    assert(speed >= 0.0 && speed <= 1.0);
    left.set_speed(speed);
}

f64 io_driver::right_motor() const
{
    return right.speed();
}

void io_driver::set_right_motor(f64 speed)
{
    assert(speed >= 0.0 && speed <= 1.0);
    right.set_speed(speed);
}

bool io_driver::heartbeat() const
{
    return heartbeat_pin.state();
}

void io_driver::set_heartbeat(bool on)
{
    heartbeat_pin.set_state(on);
}

bool io_driver::fan() const
{
    return fan_pin.state();
}

void io_driver::set_fan(bool on)
{
    fan_pin.set_state(on);
}

bool io_driver::eyes() const
{
    return eyes_pin.state();
}

void io_driver::set_eyes(bool on)
{
    eyes_pin.set_state(on);
}

f64 io_driver::temperature() const
{
    return (adc.read_voltage(0) - 0.5) * 100.0;
}

f64 io_driver::turbidity() const
{
    f64 voltage = adc.read_voltage(1) / voltage_divider(33.0, 56.0);
    if (voltage <= 2.5)
    {
        BOOST_LOG_TRIVIAL(warning) << "turbidity value out of sensor range";
        return 3000.0;
    }
    return -1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9;
}

f64 io_driver::dust(u32 iterations) const
{
    f64 sum = 0.0;
    u32 valid_measurements = 0;

    for (u32 i = 0; i < iterations; ++i)
    {
        // Flash IR and read voltage
        dust_pin.set_state(true);
        usleep(280);
        f64 voltage = adc.read_voltage(2) / voltage_divider(33.0, 18.0);
        dust_pin.set_state(false);

        // Add to average if measurement is valid
        voltage -= 0.6;
        if (voltage > 0.0)
        {
            ++valid_measurements;
            sum += voltage * 200.0;
        }
    }

    if (valid_measurements == 0)
    {
        BOOST_LOG_TRIVIAL(warning)
            << "failed to measure dust after " << iterations << " iterations";
        return 0.0;
    }

    return sum / static_cast<f64>(valid_measurements);
}

f64 io_driver::battery_voltage() const
{
    return adc.read_voltage(3) / voltage_divider(66.750, 23.870);
}

weather_data io_driver::weather() const
{
    return weather_station.get_sensor_data();
}

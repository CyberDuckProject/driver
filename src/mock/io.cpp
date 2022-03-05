#include "io.h"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <unistd.h>
#include <random>

float random_reading() noexcept
{
    static std::mt19937 gen{std::random_device{}()};
    return std::uniform_real_distribution<float>{0.0f, 1.0f}(gen);
}

void io::set_left_motor(f32 speed)
{
    assert(speed >= 0.0f && speed <= 1.0f);
    current_left_speed = speed;
    BOOST_LOG_TRIVIAL(info)  << "left motor speed set to " << speed;
}

f32 io::left_motor() const
{
    return current_left_speed;
}

void io::set_right_motor(f32 speed)
{
    assert(speed >= 0.0f && speed <= 1.0f);
    current_right_speed = speed;
    BOOST_LOG_TRIVIAL(info)  << "right motor speed set to " << speed;
}

f32 io::right_motor() const
{
    return current_right_speed;
}

void io::set_eyes(bool on)
{
    current_eyes_state = on;
    BOOST_LOG_TRIVIAL(info)  << "eyes state set to " << on;
}

bool io::eyes() const
{
    return current_eyes_state;
}

void io::set_fan(bool on)
{
    current_fan_state = on;
    BOOST_LOG_TRIVIAL(info)  << "fan state set to " << on;
}

bool io::fan() const
{
    return current_fan_state;
}

void io::set_heartbeat(bool on)
{
    current_heartbeat_state = on;
    BOOST_LOG_TRIVIAL(info)  << "heartbeat state set to " << on;
}

bool io::heartbeat() const
{
    return current_heartbeat_state;
}

f32 io::temperature()
{
    return random_reading();
}

f32 io::turbidity()
{
   return random_reading();
}

f32 io::dust()
{
    return random_reading();
}

f32 io::battery_voltage()
{
    return random_reading();
}

io::weather_redout io::weather()
{
    return {
        random_reading(),
        random_reading(),
        random_reading()
    };
}

#ifndef IO_DRIVER_H
#define IO_DRIVER_H

#include "devices.h"

class io_driver
{
public:
    f64 left_motor() const;
    void set_left_motor(f64 speed);

    f64 right_motor() const;
    void set_right_motor(f64 speed);

    bool heartbeat() const;
    void set_heartbeat(bool on);

    bool fan() const;
    void set_fan(bool on);

    bool eyes() const;
    void set_eyes(bool on);

    f64 temperature() const;
    f64 turbidity() const;
    f64 dust(u32 iterations = 10) const;
    f64 battery_voltage() const;

    weather_data weather() const;

private:
    motor left{13};
    motor right{12};
    binary_out heartbeat_pin{4, true};
    binary_out fan_pin{7, true};
    binary_out eyes_pin{6, true};
    mcp3008 adc{false, 0, 3.3};
    mutable binary_out dust_pin{5};
    bme280 weather_station{22, 0x76};
};

#endif

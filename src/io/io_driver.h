#ifndef IO_IO_DRIVER_H
#define IO_IO_DRIVER_H

#include "devices.h"

class io_driver
{
public:
    f64 temperature() const;
    f64 turbidity() const;
    f64 dust() const;
    f64 battery_voltage() const;

    bme280_data weather() const;

private:
    esc left{13};
    esc right{12};
    binary_pin heartbeat{4, true};
    binary_pin fan{7, true};
    binary_pin eyes{6, true};
    mcp3008 adc{false, 0, 3.3};
    mutable binary_pin dust_ir{5};
    bme280 humidity{22, 0x76};
};

#endif

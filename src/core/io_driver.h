#ifndef CORE_IO_DRIVER_H
#define CORE_IO_DRIVER_H

#include "dev/binary_pin.h"
#include "dev/bme280.h"
#include "dev/esc.h"
#include "dev/mcp3008.h"

class io_driver
{
public:
private:
    esc left{13};
    esc right{12};
    binary_pin heartbeat{4, true};
    binary_pin fan{7, true};
    binary_pin eyes{6, true};
    mcp3008 adc{false, 0, 3.3};
    mutable binary_pin dust{5};
    bme280 humidity{22, 0x76};
};

#endif

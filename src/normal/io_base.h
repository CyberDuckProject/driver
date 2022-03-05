#ifndef IO_BASE_H
#define IO_BASE_H

#include "peripherals.h"

struct io_base
{
public:
    peripherals::motor left{13};
    peripherals::motor right{12};
    peripherals::output_pin eyes_pin{6};
    peripherals::output_pin fan_pin{7};
    peripherals::output_pin heartbeat_pin{4};
    peripherals::output_pin dust_pin{5};
    peripherals::mcp3008 adc{false, 0, 3.3f};
    peripherals::bme280 bme280{22, 0x76, {19, 21}}; 
};

#endif
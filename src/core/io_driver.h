#ifndef CORE_IO_DRIVER_H
#define CORE_IO_DRIVER_H

#include "dev/binary_pin.h"
#include "dev/bme280.h"
#include "dev/esc.h"
#include "dev/mcp3008.h"

namespace core {

class io_driver
{
public:
private:
    dev::esc left{13};
    dev::esc right{12};
    dev::binary_pin heartbeat{4, true};
    dev::binary_pin fan{7, true};
    dev::binary_pin eyes{6, true};
    dev::mcp3008 mcp3008{false, 0, 3.3};
    mutable dev::binary_pin dust{5};
    dev::bme280 bme280{22, 0x76};
};

} // namespace core

#endif

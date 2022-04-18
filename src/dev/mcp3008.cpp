#include "mcp3008.h"

#include <cassert>
#include <pigpio.h>

mcp3008::mcp3008(bool aux, u32 channel, f64 vref) : vref{vref}
{
    handle = spiOpen(channel, 1000000, PI_SPI_FLAGS_AUX_SPI(aux));
}

mcp3008::~mcp3008()
{
    if (handle >= 0)
    {
        spiClose(handle);
    }
}

f64 mcp3008::read_voltage(u32 channel) const
{
    // The MCP3008 chip has 8 channels
    assert(channel <= 7);

    // Single-ended channel selection
    char tx[3], rx[3];
    tx[0] = 1;
    tx[1] = (8 + channel) << 4;
    spiXfer(handle, tx, rx, 3);

    // Assemble digital output code
    u32 code = (rx[1] << 8) | rx[2];
    code &= 0x3FF;

    // Calculate input voltage
    return static_cast<f64>(code) * vref / 1023.0;
}

#ifndef DEV_MCP3008_H
#define DEV_MCP3008_H

#include "utl/fundamental_types.h"

class mcp3008
{
public:
    mcp3008(bool aux, u32 channel, f64 vref);
    ~mcp3008();

    f64 read_voltage(u32 channel) const;

private:
    f64 vref;
    i32 handle;
};

#endif

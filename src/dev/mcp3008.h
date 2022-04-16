#ifndef DEV_MCP3008_H
#define DEV_MCP3008_H

#include "fundamental_types.h"

namespace dev {

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

} // namespace dev

#endif

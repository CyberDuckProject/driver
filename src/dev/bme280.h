#ifndef DEV_BME280_H
#define DEV_BME280_H

#include "utl/fundamental_types.h"
#include <bme280_defs.h>

namespace dev {

class bme280
{
public:
    bme280(u32 bus, u32 address);
    ~bme280();

    bme280_data get_sensor_data() const;

private:
    i8 read(u8 reg_addr, u8* reg_data, u32 len);
    i8 write(u8 reg_addr, const u8* reg_data, u32 len);

    i32 handle;
    mutable bme280_dev device;
};

} // namespace dev

#endif

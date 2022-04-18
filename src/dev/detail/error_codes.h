#ifndef DEV_DETAIL_ERROR_CODES_H
#define DEV_DETAIL_ERROR_CODES_H

#include "utl/fundamental_types.h"
#include <system_error>

std::error_code make_bme280_error(i8 ec);
std::error_code make_pigpio_error(i32 ec);

inline void bme280_verify(i8 ec)
{
    if (ec < 0)
    {
        throw std::system_error{make_bme280_error(ec)};
    }
}

inline void pigpio_verify(i32 ec)
{
    if (ec < 0)
    {
        throw std::system_error{make_pigpio_error(ec)};
    }
}

#endif

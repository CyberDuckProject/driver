#ifndef IO_ERROR_CODES_H
#define IO_ERROR_CODES_H

#include "utl/fundamental_types.h"
#include <system_error>

namespace io {

std::error_code make_bme280_error(i8 ec);
std::error_code make_pigpio_error(i32 ec);

} // namespace io

#endif

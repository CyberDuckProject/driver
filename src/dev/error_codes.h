#ifndef DEV_ERROR_CODES_H
#define DEV_ERROR_CODES_H

#include "fundamental_types.h"
#include <system_error>

namespace dev {

std::error_code make_bme280_error(i8 ec);
std::error_code make_pigpio_error(i32 ec);

} // namespace dev

#endif

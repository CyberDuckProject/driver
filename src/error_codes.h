#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#include "common_headers.h"

std::error_code make_pigpio_error(i32 ec);
std::error_code make_bme280_error(i8 ec);

#endif

#ifndef PERIPHERAL_ERROR_H
#define PERIPHERAL_ERROR_H

#include "fundamental_types.h"
#include <system_error>

class peripheral_error : public std::system_error
{
public:
    explicit peripheral_error(i32 error_code, bool bme280);
};

#endif

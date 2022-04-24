#ifndef MSG_ERROR_H
#define MSG_ERROR_H

#include "utl/fundamental_types.h"
#include <boost/system.hpp>

namespace msg::error {

enum message_errors : i32
{
    invalid_format = 1
};

boost::system::error_code make_error_code(message_errors ec);

} // namespace msg::error

namespace boost::system {

template<>
struct is_error_code_enum<msg::error::message_errors>
{
    static const bool value = true;
};

} // namespace boost::system

#endif

#ifndef NET_ERROR_H
#define NET_ERROR_H

#include "utl/fundamental_types.h"
#include <boost/system.hpp>

namespace net::error {

enum message_errors : i32
{
    invalid_format = 1
};

boost::system::error_code make_error_code(message_errors ec);

} // namespace net::error

namespace boost::system {

template<>
struct is_error_code_enum<net::error::message_errors>
{
    static const bool value = true;
};

} // namespace boost::system

#endif

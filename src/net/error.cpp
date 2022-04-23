#include "error.h"

namespace net::error {

class message_category : public boost::system::error_category
{
public:
    const char* name() const noexcept override
    {
        return "net.message";
    }

    std::string message(i32 ec) const override
    {
        switch (ec)
        {
        case invalid_format:
            return "invalid format";
        default:
            return "net.message error";
        }
    }
};

boost::system::error_code make_error_code(message_errors ec)
{
    static message_category instance;
    return boost::system::error_code{ec, instance};
}

} // namespace net::error

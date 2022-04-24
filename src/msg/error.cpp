#include "error.h"

namespace msg::error {

class read_category : public boost::system::error_category
{
public:
    const char* name() const noexcept override
    {
        return "msg.read";
    }

    std::string message(i32 ec) const override
    {
        switch (ec)
        {
        case invalid_format:
            return "invalid format";
        default:
            return "msg.read error";
        }
    }
};

boost::system::error_code make_error_code(read_errors ec)
{
    static read_category instance;
    return boost::system::error_code{ec, instance};
}

} // namespace msg::error

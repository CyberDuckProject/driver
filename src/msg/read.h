#ifndef MSG_READ_H
#define MSG_READ_H

#include "error.h"
#include "message_buffer.h"
#include "utl/allocate_from_token.h"
#include <boost/asio.hpp>

namespace msg {
namespace detail {

template<typename AsyncReadStream, typename... Messages>
class async_read_message_op
{
public:
    async_read_message_op(AsyncReadStream& stream, std::shared_ptr<message_type> header,
                          message_buffer<Messages...>& body) :
        stream{stream},
        header{std::move(header)}, body{body}
    {
    }

    template<typename Self>
    void operator()(Self& self, boost::system::error_code ec = {}, std::size_t n = 0)
    {
        switch (state)
        {
        case starting:
            state = reading_header;
            read_header(self);
            break;
        case reading_header:
            state = reading_body;
            read_body(self, ec, n);
            break;
        case reading_body:
            complete(self, ec, sizeof(header) + n);
            break;
        }
    }

private:
    enum op_state
    {
        starting,
        reading_header,
        reading_body
    };

    template<typename Self>
    void read_header(Self& self)
    {
        assert(header);
        boost::asio::async_read(stream, boost::asio::buffer(header.get(), sizeof(message_type)),
                                std::move(self));
    }

    template<typename Self>
    void read_body(Self& self, boost::system::error_code ec, std::size_t n)
    {
        if (ec)
        {
            complete(self, ec, n);
            return;
        }

        if (!body.try_construct(*header))
        {
            complete(self, error::invalid_format, n);
            return;
        }

        boost::asio::async_read(stream, body.data(), std::move(self));
    }

    template<typename Self>
    void complete(Self& self, boost::system::error_code ec, std::size_t n)
    {
        header.reset();
        self.complete(ec, n);
    }

    op_state state{starting};
    AsyncReadStream& stream;
    std::shared_ptr<message_type> header;
    message_buffer<Messages...>& body;
};

} // namespace detail

template<typename AsyncReadStream, typename... Messages, typename CompletionToken>
auto async_read_message(AsyncReadStream& stream, message_buffer<Messages...>& buffer,
                        CompletionToken&& token)
{
    using signature = void(boost::system::error_code, std::size_t);

    // Allocate header buffer
    std::shared_ptr<message_type> header{utl::allocate_from_token<message_type, signature>(token)};

    // Initiate composed operation
    detail::async_read_message_op impl{stream, std::move(header), buffer};
    return boost::asio::async_compose<CompletionToken, signature>(std::move(impl), token);
}

} // namespace msg

#endif

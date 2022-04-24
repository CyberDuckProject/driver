#ifndef NET_ASYNC_READ_MESSAGE_H
#define NET_ASYNC_READ_MESSAGE_H

#include "error.h"
#include "message_buffer.h"

namespace net {
namespace detail {

template<typename AsyncReadStream, typename... Messages>
class async_read_message_op
{
public:
    async_read_message_op(AsyncReadStream& stream, message_buffer<Messages...>& body) :
        stream{stream}, body{body}
    {
    }

    template<typename Self>
    void operator()(Self& self, boost::system::error_code ec = {}, std::size_t n = {})
    {
        switch (state)
        {
        case op_state::starting:
            state = op_state::reading_header;
            read_header(self);
            break;
        case op_state::reading_header:
            state = op_state::reading_body;
            read_body_or_complete(self, ec, n);
            break;
        case op_state::reading_body:
            self.complete(std::move(ec), sizeof(header) + n);
            break;
        }
    }

private:
    enum class op_state
    {
        starting,
        reading_header,
        reading_body
    };

    template<typename Self>
    void read_header(Self& self)
    {
        boost::asio::async_read(stream, boost::asio::buffer(&header, sizeof(header)),
                                std::move(self));
    }

    template<typename Self>
    void read_body_or_complete(Self& self, boost::system::error_code ec, std::size_t n)
    {
        if (ec)
        {
            self.complete(ec, n);
            return;
        }

        if (!body.try_emplace(header))
        {
            self.complete(error::invalid_format, n);
            return;
        }
        
        boost::asio::async_read(stream, body.data(), std::move(self));
    }

    op_state state{op_state::starting};
    AsyncReadStream& stream;
    message_type header;
    message_buffer<Messages...>& body;
};

} // namespace detail

template<typename AsyncReadStream, typename... Messages, typename CompletionToken>
auto async_read_message(AsyncReadStream& stream, message_buffer<Messages...>& buffer,
                        CompletionToken&& token)
{
    return boost::asio::async_compose<CompletionToken,
                                      void(boost::system::error_code, std::size_t)>(
        detail::async_read_message_op{stream, buffer}, token, stream);
}

} // namespace net

#endif

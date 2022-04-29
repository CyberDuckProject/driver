#ifndef MSG_WRITE_H
#define MSG_WRITE_H

#include "message_buffer.h"
#include "utl/allocate_from_token.h"
#include <boost/asio.hpp>

namespace msg {
namespace detail {

template<typename AsyncWriteStream, typename... Messages>
class async_write_message_op
{
public:
    async_write_message_op(AsyncWriteStream& stream, std::shared_ptr<message_type> header,
                           const message_buffer<Messages...>& body) :
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
            state = writing_header;
            write_header(self);
            break;
        case writing_header:
            state = writing_body;
            write_body(self, ec, n);
            break;
        case writing_body:
            complete(self, ec, sizeof(message_type) + n);
            break;
        }
    }

private:
    enum op_state
    {
        starting,
        writing_header,
        writing_body
    };

    template<typename Self>
    void write_header(Self& self)
    {
        assert(header);
        boost::asio::async_write(stream, boost::asio::buffer(header.get(), sizeof(message_type)),
                                 std::move(self));
    }

    template<typename Self>
    void write_body(Self& self, boost::system::error_code ec, std::size_t n)
    {
        if (ec)
        {
            complete(ec, n);
            return;
        }

        boost::asio::async_write(stream, body.data(), std::move(self));
    }

    template<typename Self>
    void complete(Self& self, boost::system::error_code ec, std::size_t n)
    {
        header.reset();
        self.complete(ec, n);
    }

    op_state state{starting};
    AsyncWriteStream& stream;
    std::shared_ptr<message_type> header;
    const message_buffer<Messages...>& body;
};

} // namespace detail

template<typename AsyncWriteStream, typename... Messages, typename CompletionToken>
auto async_write_message(AsyncWriteStream& stream, const message_buffer<Messages...>& buffer,
                         CompletionToken&& token)
{
    using signature = void(boost::system::error_code, std::size_t);

    // Allocate header buffer
    std::shared_ptr<message_type> header{utl::allocate_from_token<message_type, signature>(token)};

    // Fill header with message type
    assert(!buffer.empty());
    *header = buffer.type();

    // Initiate composed operation
    detail::async_write_message_op impl{stream, std::move(header), buffer};
    return boost::asio::async_compose<CompletionToken, signature>(std::move(impl), token);
}

} // namespace msg

#endif

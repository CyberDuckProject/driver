#ifndef MSG_ASYNC_WRITE_MESSAGE_H
#define MSG_ASYNC_WRITE_MESSAGE_H

#include "message_buffer.h"
#include <boost/asio.hpp>

namespace msg {
namespace detail {

template<typename AsyncWriteStream, typename... Messages>
class async_write_message_op
{
public:
    async_write_message_op(AsyncWriteStream& stream, const message_buffer<Messages...>& buffer) :
        stream{stream}, header{buffer.type()}, body{buffer}
    {
    }

    template<typename Self>
    void operator()(Self& self, boost::system::error_code ec = {}, std::size_t n = 0)
    {
        switch (state)
        {
        case op_state::starting:
            state = op_state::writing_header;
            write_header(self);
            break;
        case op_state::writing_header:
            state = op_state::writing_body;
            write_body_or_complete(self, std::move(ec), n);
            break;
        case op_state::writing_body:
            self.complete(std::move(ec), sizeof(header) + n);
            break;
        }
    }

private:
    enum class op_state
    {
        starting,
        writing_header,
        writing_body
    };

    template<typename Self>
    void write_header(Self& self)
    {
        boost::asio::async_write(stream, boost::asio::buffer(&header, sizeof(header)),
                                 std::move(self));
    }

    template<typename Self>
    void write_body_or_complete(Self& self, boost::system::error_code ec, std::size_t n)
    {
        if (ec)
        {
            self.complete(std::move(ec), n);
            return;
        }

        boost::asio::async_write(stream, body.data(), std::move(self));
    }

    op_state state{op_state::starting};
    AsyncWriteStream& stream;
    message_type header;
    const message_buffer<Messages...>& body;
};

} // namespace detail

template<typename AsyncWriteStream, typename... Messages, typename CompletionToken>
auto async_write_message(AsyncWriteStream& stream, const message_buffer<Messages...>& buffer,
                         CompletionToken&& token)
{
    return boost::asio::async_compose<CompletionToken,
                                      void(boost::system::error_code, std::size_t)>(
        detail::async_write_message_op{stream, buffer}, token, stream);
}

} // namespace msg

#endif

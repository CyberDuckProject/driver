#ifndef NET_CONNECTION_H
#define NET_CONNECTION_H

#include "msg/message_buffer.h"
#include <boost/asio.hpp>

namespace net {

template<typename ExecutionContext>
class connection
{
public:
    connection(ExecutionContext& ctx, u16 port) :
        acceptor{ctx, {boost::asio::ip::tcp::v4(), port}}, socket{ctx}
    {
    }

    template<typename CompletionToken>
    auto async_accept(CompletionToken&& token)
    {
    }

    template<typename... Messages, typename CompletionToken>
    auto async_read_message(msg::message_buffer<Messages...>& buffer, CompletionToken&& token)
    {
    }

    template<typename... Messages, typename CompletionToken>
    auto async_write_message(const msg::message_buffer<Messages...>& buffer,
                             CompletionToken&& token)
    {
    }

private:
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
};

} // namespace net

#endif

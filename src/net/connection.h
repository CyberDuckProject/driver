#ifndef NET_CONNECTION_H
#define NET_CONNECTION_H

#include "msg/message_buffer.h"
#include <boost/asio.hpp>

namespace net {

template<typename Executor>
class connection
{
public:
    using executor_type = Executor;

    connection(const executor_type& ex, u16 port) :
        ex{ex}, acceptor{ex, {boost::asio::ip::tcp::v4(), port}}, socket{ex}
    {
    }

    executor_type get_executor()
    {
        return ex;
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
    executor_type ex;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
};

} // namespace net

#endif

#ifndef NET_CONNECTION_H
#define NET_CONNECTION_H

#include "msg/read.h"
#include "msg/write.h"
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
        boost::asio::async_completion<CompletionToken, void(boost::system::error_code)> init{token};

        acceptor.async_accept([this, handler = std::move(init.completion_handler)](
                                  boost::system::error_code ec, boost::asio::ip::tcp::socket s) {
            if (!ec)
            {
                socket = std::move(s);
            }
            handler(std::move(ec));
        });

        return init.result.get();
    }

    template<typename... Messages, typename CompletionToken>
    auto async_read_message(msg::message_buffer<Messages...>& buffer, CompletionToken&& token)
    {
        // TODO: add timeouts
        return msg::async_read_message(socket, buffer, std::forward<CompletionToken>(token));
    }

    template<typename... Messages, typename CompletionToken>
    auto async_write_message(const msg::message_buffer<Messages...>& buffer,
                             CompletionToken&& token)
    {
        // TODO: add timeouts
        return msg::async_write_message(socket, buffer, std::forward<CompletionToken>(token));
    }

private:
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
};

} // namespace net

#endif

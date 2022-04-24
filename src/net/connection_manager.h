#ifndef NET_CONNECTION_MANAGER_H
#define NET_CONNECTION_MANAGER_H

#include "async_read_message.h"
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

namespace net {

template<typename ExecutionContext>
class connection_manager
{
public:
    connection_manager(ExecutionContext& ctx, u16 port) :
        acceptor{ctx, {boost::asio::ip::tcp::v4(), port}}, socket{ctx},
        strand{boost::asio::make_strand(ctx)}
    {
        accept();
    }

private:
    void accept()
    {
        acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket s) {
            if (!ec)
            {
                BOOST_LOG_TRIVIAL(debug) << "connected to " << s.remote_endpoint();
                socket = std::move(s);
                read();
            }
            else
            {
                throw boost::system::system_error{ec};
            }
        });
    }

    void read()
    {
        async_read_message(socket, buffer, [this](boost::system::error_code ec, std::size_t n) {
            if (!ec)
            {
                // TODO: handle message
                read();
            }
            else if (ec == error::invalid_format)
            {
                BOOST_LOG_TRIVIAL(error) << "invalid message format";
                read();
            }
            else if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
            {
                BOOST_LOG_TRIVIAL(debug) << "disconnected";
                accept();
            }
            else
            {
                throw boost::system::system_error{ec};
            }
        });
    }

    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    boost::asio::strand<typename ExecutionContext::executor_type> strand;
    message_buffer<control_message> buffer;
};

} // namespace net

#endif

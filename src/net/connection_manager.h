#ifndef NET_CONNECTION_MANAGER_H
#define NET_CONNECTION_MANAGER_H

#include "msg/async_read_message.h"
#include "msg/message_buffer.h"
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

namespace net {

template<typename Executor>
class connection_manager
{
public:
    connection_manager(Executor ex, u16 port) :
        acceptor{ex, {boost::asio::ip::tcp::v4(), port}}, socket{ex}, strand{ex}
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
        msg::async_read_message(socket, buffer, [this](boost::system::error_code ec, std::size_t) {
            if (!ec)
            {
                // TODO: handle message
                read();
            }
            else if (ec == msg::error::invalid_format)
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
    boost::asio::strand<Executor> strand;
    msg::message_buffer<msg::control_message> buffer;
};

} // namespace net

#endif

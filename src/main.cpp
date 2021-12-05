#include "io.h"
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

void accept(tcp::acceptor& acceptor)
{
    // TODO: set eyes to flash
    BOOST_LOG_TRIVIAL(info) << "awaiting connection...";
    acceptor.async_accept([&](boost::system::error_code ec, tcp::socket socket) {
        if (!ec)
        {
            // TODO: set eyes to steady
            BOOST_LOG_TRIVIAL(info) << "connected to " << socket.remote_endpoint();
            while (true)
            {
                // TODO: this should be done on the main thread
                float data[2];
                boost::system::error_code ec;
                socket.read_some(asio::buffer(data), ec);

                if (!ec)
                {
                    io::set_speed(io::motor::left, data[0]);
                    io::set_speed(io::motor::right, data[1]);
                }
                else if (ec == asio::error::eof)
                {
                    BOOST_LOG_TRIVIAL(info) << "disconnected";
                    break;
                }
                else
                {
                    throw boost::system::system_error{ec};
                }
            }
        }
        accept(acceptor);
    });
}

int main()
{
    io::init();

    asio::io_context ctx{};
    tcp::acceptor acceptor{ctx, {tcp::v4(), 13}};
    accept(acceptor);
    ctx.run();

    io::shutdown();
}

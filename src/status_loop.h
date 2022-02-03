#ifndef STATUS_LOOP_H
#define STATUS_LOOP_H

#include "io.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

template<typename executor_type>
class status_loop
{
public:
    status_loop(executor_type& ctx, io& io, std::chrono::milliseconds period) :
        timer{ctx}, socket{ctx}, io{io}, period{period}
    {
        timer.async_wait(boost::bind(&status_loop::loop, this));
    }

    void connect(const boost::asio::ip::udp::endpoint& endpoint)
    {
        socket.async_connect(endpoint, [&](const boost::system::error_code& ec) {
            if (ec)
            {
                throw boost::system::system_error{ec};
            }
            connected = true;
        });
    }

private:
    void send_status()
    {
        // TODO: send sensor data
    }

    void loop()
    {
        if (connected)
        {
            if (!io.eyes())
            {
                io.set_eyes(true);
            }
            send_status();
        }
        else
        {
            io.set_eyes(!io.eyes());
        }

        timer.expires_at(timer.expiry() + period);
        timer.async_wait(boost::bind(&status_loop::loop, this));
    }

    boost::asio::steady_timer timer;
    boost::asio::ip::udp::socket socket;
    ::io& io;
    std::chrono::milliseconds period;
    std::atomic_bool connected{false};
};

#endif

#include "status_loop.h"

#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>

namespace asio = boost::asio;
using udp = asio::ip::udp;

status_loop::status_loop(asio::thread_pool& ctx, ::io& io, std::chrono::milliseconds period) :
    io{io}, timer{ctx, period}, period{period}, socket{ctx, udp::v4()}
{
    timer.async_wait(boost::bind(&status_loop::wait_handler, this));
}

void status_loop::connect(const udp::endpoint& endpoint)
{
    std::lock_guard lock{mutex};
    remote = endpoint;
}

void status_loop::disconnect()
{
    std::lock_guard lock{mutex};
    remote.reset();
}

void status_loop::wait_handler()
{
    std::lock_guard lock{mutex};
    if (remote.has_value())
    {
        if (!io.eyes())
        {
            io.set_eyes(true);
        }
        loop();
    }
    else
    {
        io.set_eyes(!io.eyes());
    }

    timer.expires_at(timer.expiry() + period);
    timer.async_wait(boost::bind(&status_loop::wait_handler, this));
}

void status_loop::loop()
{
    // TODO: send data
}

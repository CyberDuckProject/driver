#include "status_loop.h"

#include <boost/bind/bind.hpp>

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
    struct message
    {
        f32 water_temperature;
        f32 turbidity;
        f32 dust;
        f32 battery_voltage;
        f32 pressure;
        f32 temperature;
        f32 humidity;
    };

    message msg;
    msg.water_temperature = io.temperature();
    msg.turbidity = io.turbidity();
    msg.dust = io.dust();
    msg.battery_voltage = io.battery_voltage();

    auto weather = io.weather();
    msg.pressure = weather.pressure;
    msg.temperature = weather.temperature;
    msg.humidity = weather.humidity;

    assert(remote.has_value());
    socket.async_send_to(asio::buffer(&msg, sizeof(msg)), *remote, [](...) {});
}

#include "transmitter.h"

namespace asio = boost::asio;

void transmitter::send_to(const endpoint_type& remote)
{
    struct message
    {
        f64 temperature;
        f64 turbidity;
        f64 dust;
        f64 battery_voltage;
        weather_data weather;
    };

    message msg;
    msg.temperature = drv.temperature();
    msg.turbidity = drv.turbidity();
    msg.dust = drv.dust();
    msg.battery_voltage = drv.battery_voltage();
    msg.weather = drv.weather();

    socket.async_send_to(asio::buffer(&msg, sizeof(msg)), remote, [](...) {});
}

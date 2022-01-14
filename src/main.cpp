#include "io.h"
#include "net.h"
#include <boost/log/trivial.hpp>

int main()
{
    io::init();

    net::server server{1333, 1512};

    while (true)
    {
        // TODO: add support for different modes
        net::config config{server.pop_config()};

        config.handle<net::config::empty>([&](...) {
            if (server.connected())
                server.send(net::sensor_type::water_temperature, io::water_temperature());
        });

        config.handle<net::config::manual>([](net::config::manual cfg) {
            io::set_left_motor(cfg.left);
            io::set_right_motor(cfg.right);
        });
    }

    io::shutdown();
}

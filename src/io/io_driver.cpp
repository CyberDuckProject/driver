#include "io_driver.h"

#include <boost/log/trivial.hpp>

namespace io {

f64 io_driver::temperature() const
{
    return (adc.read_voltage(0) - 0.5) * 100.0;
}

f64 io_driver::turbidity() const
{
    // Read voltage
    f64 voltage = adc.read_voltage(1) / voltage_ratio(33.0, 56.0);

    // Check if measurement was valid
    if (voltage < 2.5 || voltage > 4.2)
    {
        BOOST_LOG_TRIVIAL(warning) << "turbidity out of range";
        return 0.0;
    }
    return -1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9;
}

f64 io_driver::dust() const
{
    // Flash IR and read voltage
    dust_ir.set_state(true);
    usleep(280);
    f64 voltage = adc.read_voltage(2) / voltage_ratio(33.0, 18.0);
    dust_ir.set_state(false);

    // Check if measurement was valid
    voltage -= 0.6;
    if (voltage <= 0.0)
    {
        BOOST_LOG_TRIVIAL(warning) << "dust out of range";
        return 0.0;
    }
    return voltage * 200.0;
}

f64 io_driver::battery_voltage() const
{
    return adc.read_voltage(3) / voltage_ratio(66.750, 23.870);
}

bme280_data io_driver::weather() const
{
    return humidity.get_sensor_data();
}

} // namespace io

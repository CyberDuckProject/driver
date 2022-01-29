#include "peripherals.h"

int main()
{
    motor left{13};
    motor right{12};
    output_pin eyes{6};
    output_pin fan{7};
    adc ain{false, 0, 3.3f};
    // bme280 env{2, 0x76, {21, 19}};
}

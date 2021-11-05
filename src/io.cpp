#include "io.h"

#include <cassert>
#include <pigpio.h>

#define GPIO_CALL(call)                                                                            \
	do                                                                                             \
	{                                                                                              \
		if (i32 code = call; code < 0)                                                             \
		{                                                                                          \
			throw gpio_error{code};                                                                \
		}                                                                                          \
	}                                                                                              \
	while (false)

namespace io {
namespace {

constexpr u32 adc_baud{1000000};
constexpr f32 adc_ref{3.3f};
i32 adc;

constexpr u8 temp_channel{0};

f32 read_adc(u8 channel)
{
	assert(channel <= 7);

	char buf[3]{1, static_cast<char>((8 + channel) << 4), 0};
	char readBuf[3];
	GPIO_CALL(spiXfer(adc, buf, readBuf, 3));

	i32 value{((readBuf[1] << 8) | readBuf[2]) & 0x3FF};
	return static_cast<f32>(value) * adc_ref / 1023.0f;
}

} // namespace

void init()
{
	// Disable printing
	u32 internals{gpioCfgGetInternals() | PI_CFG_NOSIGHANDLER};
	gpioCfgSetInternals(internals);

	GPIO_CALL(gpioInitialise());
	GPIO_CALL(adc = spiOpen(0, adc_baud, 0));
}

void shutdown()
{
	if (adc > 0)
	{
		spiClose(adc);
	}

	gpioTerminate();
}

f32 water_temperature()
{
	f32 volts{read_adc(temp_channel)};
	return (volts - 0.5f) * 100.0f;
}

} // namespace io

#include "error_codes.h"

#include <bme280_defs.h>
#include <pigpio.h>

class bme280_category : public std::error_category
{
public:
    const char* name() const noexcept override
    {
        return "bme280";
    }

    std::string message(i32 condition) const override
    {
        switch (condition)
        {
        case BME280_OK:
            return "success";
        case BME280_E_NULL_PTR:
            return "null pointer";
        case BME280_E_DEV_NOT_FOUND:
            return "device not found";
        case BME280_E_INVALID_LEN:
            return "invalid length";
        case BME280_E_COMM_FAIL:
            return "communication failed";
        case BME280_E_SLEEP_MODE_FAIL:
            return "sleep mode failed";
        case BME280_E_NVM_COPY_FAILED:
            return "NVM copy failed";
        default:
            return "unknown error";
        }
    }
};

class pigpio_category : public std::error_category
{
public:
    const char* name() const noexcept override
    {
        return "pigpio";
    }

    std::string message(i32 condition) const override
    {
        switch (condition)
        {
        case 0:
            return "success";
        case PI_INIT_FAILED:
            return "gpioInitialise failed";
        case PI_BAD_USER_GPIO:
            return "GPIO not 0-31";
        case PI_BAD_GPIO:
            return "GPIO not 0-53";
        case PI_BAD_MODE:
            return "mode not 0-7";
        case PI_BAD_LEVEL:
            return "level not 0-1";
        case PI_BAD_PUD:
            return "pud not 0-2";
        case PI_BAD_PULSEWIDTH:
            return "pulsewidth not 0 or 500-2500";
        case PI_BAD_DUTYCYCLE:
            return "dutycycle outside set range";
        case PI_BAD_TIMER:
            return "timer not 0-9";
        case PI_BAD_MS:
            return "ms not 10-60000";
        case PI_BAD_TIMETYPE:
            return "timetype not 0-1";
        case PI_BAD_SECONDS:
            return "seconds < 0";
        case PI_BAD_MICROS:
            return "micros not 0-999999";
        case PI_TIMER_FAILED:
            return "gpioSetTimerFunc failed";
        case PI_BAD_WDOG_TIMEOUT:
            return "timeout not 0-60000";
        case PI_BAD_CLK_PERIPH:
            return "clock peripheral not 0-1";
        case PI_BAD_CLK_MICROS:
            return "clock micros not 1, 2, 4, 5, 8, or 10";
        case PI_BAD_BUF_MILLIS:
            return "buf millis not 100-10000";
        case PI_BAD_DUTYRANGE:
            return "dutycycle range not 25-40000";
        case PI_BAD_SIGNUM:
            return "signum not 0-63";
        case PI_BAD_PATHNAME:
            return "can't open pathname";
        case PI_NO_HANDLE:
            return "no handle available";
        case PI_BAD_HANDLE:
            return "unknown handle";
        case PI_BAD_IF_FLAGS:
            return "ifFlags > 4";
        case PI_BAD_CHANNEL:
            return "DMA channel not 0-15";
        case PI_BAD_SOCKET_PORT:
            return "socket port not 1024-32000";
        case PI_BAD_FIFO_COMMAND:
            return "unrecognized fifo command";
        case PI_BAD_SECO_CHANNEL:
            return "DMA secondary channel not 0-15";
        case PI_NOT_INITIALISED:
            return "function called before gpioInitialise";
        case PI_INITIALISED:
            return "function called after gpioInitialise";
        case PI_BAD_WAVE_MODE:
            return "waveform mode not 0-3";
        case PI_BAD_CFG_INTERNAL:
            return "bad parameter in gpioCfgInternals call";
        case PI_BAD_WAVE_BAUD:
            return "baud rate not 50-250K(RX)/50-1M(TX)";
        case PI_TOO_MANY_PULSES:
            return "waveform has too many pulses";
        case PI_TOO_MANY_CHARS:
            return "waveform has too many chars";
        case PI_NOT_SERIAL_GPIO:
            return "no bit bang serial read on GPIO";
        case PI_BAD_SERIAL_STRUC:
            return "bad (null) serial structure parameter";
        case PI_BAD_SERIAL_BUF:
            return "bad (null) serial buf parameter";
        case PI_NOT_PERMITTED:
            return "GPIO operation not permitted";
        case PI_SOME_PERMITTED:
            return "one or more GPIO not permitted";
        case PI_BAD_WVSC_COMMND:
            return "bad WVSC subcommand";
        case PI_BAD_WVSM_COMMND:
            return "bad WVSM subcommand";
        case PI_BAD_WVSP_COMMND:
            return "bad WVSP subcommand";
        case PI_BAD_PULSELEN:
            return "trigger pulse length not 1-100";
        case PI_BAD_SCRIPT:
            return "invalid script";
        case PI_BAD_SCRIPT_ID:
            return "unknown script id";
        case PI_BAD_SER_OFFSET:
            return "add serial data offset > 30 minutes";
        case PI_GPIO_IN_USE:
            return "GPIO already in use";
        case PI_BAD_SERIAL_COUNT:
            return "must read at least a byte at a time";
        case PI_BAD_PARAM_NUM:
            return "script parameter id not 0-9";
        case PI_DUP_TAG:
            return "script has duplicate tag";
        case PI_TOO_MANY_TAGS:
            return "script has too many tags";
        case PI_BAD_SCRIPT_CMD:
            return "illegal script command";
        case PI_BAD_VAR_NUM:
            return "script variable id not 0-149";
        case PI_NO_SCRIPT_ROOM:
            return "no more room for scripts";
        case PI_NO_MEMORY:
            return "can't allocate temporary memory";
        case PI_SOCK_READ_FAILED:
            return "socket read failed";
        case PI_SOCK_WRIT_FAILED:
            return "socket write failed";
        case PI_TOO_MANY_PARAM:
            return "too many script parameters (> 10)";
        case PI_SCRIPT_NOT_READY:
            return "script initialising";
        case PI_BAD_TAG:
            return "script has unresolved tag";
        case PI_BAD_MICS_DELAY:
            return "bad MICS delay (too large)";
        case PI_BAD_MILS_DELAY:
            return "bad MILS delay (too large)";
        case PI_BAD_WAVE_ID:
            return "non existent wave id";
        case PI_TOO_MANY_CBS:
            return "No more CBs for waveform";
        case PI_TOO_MANY_OOL:
            return "No more OOL for waveform";
        case PI_EMPTY_WAVEFORM:
            return "attempt to create an empty waveform";
        case PI_NO_WAVEFORM_ID:
            return "no more waveforms";
        case PI_I2C_OPEN_FAILED:
            return "can't open I2C device";
        case PI_SER_OPEN_FAILED:
            return "can't open serial device";
        case PI_SPI_OPEN_FAILED:
            return "can't open SPI device";
        case PI_BAD_I2C_BUS:
            return "bad I2C bus";
        case PI_BAD_I2C_ADDR:
            return "bad I2C address";
        case PI_BAD_SPI_CHANNEL:
            return "bad SPI channel";
        case PI_BAD_FLAGS:
            return "bad i2c/spi/ser open flags";
        case PI_BAD_SPI_SPEED:
            return "bad SPI speed";
        case PI_BAD_SER_DEVICE:
            return "bad serial device name";
        case PI_BAD_SER_SPEED:
            return "bad serial baud rate";
        case PI_BAD_PARAM:
            return "bad i2c/spi/ser parameter";
        case PI_I2C_WRITE_FAILED:
            return "i2c write failed";
        case PI_I2C_READ_FAILED:
            return "i2c read failed";
        case PI_BAD_SPI_COUNT:
            return "bad SPI count";
        case PI_SER_WRITE_FAILED:
            return "ser write failed";
        case PI_SER_READ_FAILED:
            return "ser read failed";
        case PI_SER_READ_NO_DATA:
            return "ser read no data available";
        case PI_UNKNOWN_COMMAND:
            return "unknown command";
        case PI_SPI_XFER_FAILED:
            return "spi xfer/read/write failed";
        case PI_BAD_POINTER:
            return "bad (NULL) pointer";
        case PI_NO_AUX_SPI:
            return "no auxiliary SPI on Pi A or B";
        case PI_NOT_PWM_GPIO:
            return "GPIO is not in use for PWM";
        case PI_NOT_SERVO_GPIO:
            return "GPIO is not in use for servo pulses";
        case PI_NOT_HCLK_GPIO:
            return "GPIO has no hardware clock";
        case PI_NOT_HPWM_GPIO:
            return "GPIO has no hardware PWM";
        case PI_BAD_HPWM_FREQ:
            return "invalid hardware PWM frequency";
        case PI_BAD_HPWM_DUTY:
            return "hardware PWM dutycycle not 0-1M";
        case PI_BAD_HCLK_FREQ:
            return "invalid hardware clock frequency";
        case PI_BAD_HCLK_PASS:
            return "need password to use hardware clock 1";
        case PI_HPWM_ILLEGAL:
            return "illegal, PWM in use for main clock";
        case PI_BAD_DATABITS:
            return "serial data bits not 1-32";
        case PI_BAD_STOPBITS:
            return "serial (half) stop bits not 2-8";
        case PI_MSG_TOOBIG:
            return "socket/pipe message too big";
        case PI_BAD_MALLOC_MODE:
            return "bad memory allocation mode";
        case PI_TOO_MANY_SEGS:
            return "too many I2C transaction segments";
        case PI_BAD_I2C_SEG:
            return "an I2C transaction segment failed";
        case PI_BAD_SMBUS_CMD:
            return "SMBus command not supported by driver";
        case PI_NOT_I2C_GPIO:
            return "no bit bang I2C in progress on GPIO";
        case PI_BAD_I2C_WLEN:
            return "bad I2C write length";
        case PI_BAD_I2C_RLEN:
            return "bad I2C read length";
        case PI_BAD_I2C_CMD:
            return "bad I2C command";
        case PI_BAD_I2C_BAUD:
            return "bad I2C baud rate, not 50-500k";
        case PI_CHAIN_LOOP_CNT:
            return "bad chain loop count";
        case PI_BAD_CHAIN_LOOP:
            return "empty chain loop";
        case PI_CHAIN_COUNTER:
            return "too many chain counters";
        case PI_BAD_CHAIN_CMD:
            return "bad chain command";
        case PI_BAD_CHAIN_DELAY:
            return "bad chain delay micros";
        case PI_CHAIN_NESTING:
            return "chain counters nested too deeply";
        case PI_CHAIN_TOO_BIG:
            return "chain is too long";
        case PI_DEPRECATED:
            return "deprecated function removed";
        case PI_BAD_SER_INVERT:
            return "bit bang serial invert not 0 or 1";
        case PI_BAD_EDGE:
            return "bad ISR edge value, not 0-2";
        case PI_BAD_ISR_INIT:
            return "bad ISR initialisation";
        case PI_BAD_FOREVER:
            return "loop forever must be last command";
        case PI_BAD_FILTER:
            return "bad filter parameter";
        case PI_BAD_PAD:
            return "bad pad number";
        case PI_BAD_STRENGTH:
            return "bad pad drive strength";
        case PI_FIL_OPEN_FAILED:
            return "file open failed";
        case PI_BAD_FILE_MODE:
            return "bad file mode";
        case PI_BAD_FILE_FLAG:
            return "bad file flag";
        case PI_BAD_FILE_READ:
            return "bad file read";
        case PI_BAD_FILE_WRITE:
            return "bad file write";
        case PI_FILE_NOT_ROPEN:
            return "file not open for read";
        case PI_FILE_NOT_WOPEN:
            return "file not open for write";
        case PI_BAD_FILE_SEEK:
            return "bad file seek";
        case PI_NO_FILE_MATCH:
            return "no files match pattern";
        case PI_NO_FILE_ACCESS:
            return "no permission to access file";
        case PI_FILE_IS_A_DIR:
            return "file is a directory";
        case PI_BAD_SHELL_STATUS:
            return "bad shell return status";
        case PI_BAD_SCRIPT_NAME:
            return "bad script name";
        case PI_BAD_SPI_BAUD:
            return "bad SPI baud rate, not 50-500k";
        case PI_NOT_SPI_GPIO:
            return "no bit bang SPI in progress on GPIO";
        case PI_BAD_EVENT_ID:
            return "bad event id";
        case PI_CMD_INTERRUPTED:
            return "Used by Python";
        case PI_NOT_ON_BCM2711:
            return "not available on BCM2711";
        case PI_ONLY_ON_BCM2711:
            return "only available on BCM2711";
        default:
            return "unknown error";
        }
    }
};

std::error_code make_bme280_error(i8 ec)
{
    static bme280_category category;
    return std::error_code{ec, category};
}

std::error_code make_pigpio_error(i32 ec)
{
    static pigpio_category category;
    return std::error_code{ec, category};
}

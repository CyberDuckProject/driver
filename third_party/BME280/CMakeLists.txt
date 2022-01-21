add_library(BME280 STATIC
    include/bme280_defs.h
    include/bme280.h
    src/bme280.c
)

# third_party namespace
add_library(third_party::BME280 ALIAS BME280)

# Include directory
target_include_directories(BME280 PUBLIC include)

add_library(BME280_driver STATIC
    include/bme280_defs.h
    include/bme280.h
    src/bme280.c
)

# third_party namespace
add_library(third_party::BME280_driver ALIAS BME280_driver)

# Include directory
target_include_directories(BME280_driver PUBLIC include)

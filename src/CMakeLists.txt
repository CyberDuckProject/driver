add_executable(driver
    main.cpp
    fundamental_types.h
    dev/bme280.h
    dev/bme280.cpp
    dev/mcp3008.h
    dev/mcp3008.cpp
    dev/esc.h
    dev/esc.cpp
    dev/binary_pin.h
    dev/binary_pin.cpp
)

# Include directory
target_include_directories(driver PRIVATE .)

# Find requried Boost components
find_package(Boost REQUIRED
    thread system log
)

# Link against libraries
target_link_libraries(driver PRIVATE
    Boost::thread Boost::system Boost::log
    third_party::BME280_driver
    third_party::pigpio
)

# Compile definitions
target_compile_definitions(driver PRIVATE BOOST_LOG_DYN_LINK)

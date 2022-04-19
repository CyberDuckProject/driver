add_executable(driver
    io/devices.cpp
    io/devices.h
    io/error_codes.cpp
    io/error_codes.h
    io/io_driver.cpp
    io/io_driver.h
    utl/fundamental_types.h
    main.cpp
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

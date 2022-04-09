add_executable(driver
    main.cpp
    common_headers.h
    error_codes.h
    error_codes.cpp
    devices.h
    devices.cpp
    io_driver.h
    io_driver.cpp
    network.h
    network.cpp
)

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

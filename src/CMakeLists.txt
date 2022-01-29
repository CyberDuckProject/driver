add_executable(driver
    main.cpp
    fundamental_types.h
    peripheral_error.h
    peripheral_error.cpp
    peripherals.h
    peripherals.cpp
    net.h
    net.cpp
)

# Find dependencies
find_package(pigpio REQUIRED)
find_package(Boost REQUIRED
    thread system log
)

# Link against libraries
target_link_libraries(driver PRIVATE
    pigpio::pigpio
    Boost::thread Boost::system Boost::log
    third_party::BME280
)

# Compile definitions
target_compile_definitions(driver PRIVATE BOOST_LOG_DYN_LINK)

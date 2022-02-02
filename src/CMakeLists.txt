add_executable(driver
    main.cpp
    fundamental_types.h
    peripheral_error.h
    peripheral_error.cpp
    peripherals.h
    peripherals.cpp
    io.h
    io.cpp
    log_setup.cpp
)

# Find dependencies
find_package(pigpio REQUIRED)
find_package(Boost REQUIRED
    thread system log log_setup
)

# Link against libraries
target_link_libraries(driver PRIVATE
    pigpio::pigpio
    Boost::thread Boost::system Boost::log Boost::log_setup
    third_party::BME280
)

# Compile definitions
target_compile_definitions(driver PRIVATE BOOST_LOG_DYN_LINK)

add_executable(driver
    main.cpp
    fundamental_types.h
    peripherals_detail.h
    peripherals_detail.cpp
    peripherals.h
    peripherals.cpp
    io.h
    io.cpp
    status_loop.h
    status_loop.cpp
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

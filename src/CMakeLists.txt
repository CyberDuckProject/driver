add_executable(driver
    main.cpp
    fundamental_types.h
    peripherals_detail.h
    peripherals.h
    io.h
    io.cpp
    status_loop.h
    status_loop.cpp
)

# Mock peripherals
if (NOT MOCK_PERIPHERALS)
    find_package(pigpio REQUIRED)
    target_link_libraries(driver PRIVATE pigpio::pigpio)
    target_sources(driver PRIVATE
        peripherals.cpp
        peripherals_detail.cpp
    )
else()
    target_sources(driver PRIVATE peripherals_mock.cpp)
endif()

# Find dependencies
find_package(Boost REQUIRED
    thread system log
)

# Link against libraries
target_link_libraries(driver PRIVATE
    Boost::thread Boost::system Boost::log
    third_party::BME280
)

# Compile definitions
target_compile_definitions(driver PRIVATE BOOST_LOG_DYN_LINK)

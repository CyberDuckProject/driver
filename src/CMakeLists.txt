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
find_package(pigpio)
find_package(Boost REQUIRED
    thread system log
)

# Enable mock mode if pigpio not found
if (NOT ${pigpio_FOUND})
    set(MOCK_MODE ON)
else()
    set(MOCK_MODE OFF)
endif()

# Link against Boost
target_link_libraries(driver PRIVATE
    Boost::thread Boost::system Boost::log
)

# Link against sensor libraries if not in mock mode
if (NOT ${MOCK_MODE})
    target_link_libraries(driver PRIVATE
        pigpio::pigpio
        third_party::BME280
    )
endif()

# Compile definitions
target_compile_definitions(driver PRIVATE BOOST_LOG_DYN_LINK)

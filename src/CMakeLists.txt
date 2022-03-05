add_executable(driver
    main.cpp
    fundamental_types.h
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

# Add implementation depending on mode
target_include_directories(driver PRIVATE .)
if (NOT ${MOCK_MODE})
    target_sources(driver PRIVATE
        normal/peripherals_detail.h
        normal/peripherals_detail.cpp
        normal/peripherals.h
        normal/peripherals.cpp
        normal/io_base.h
        normal/io.cpp
    )
    target_include_directories(driver PRIVATE normal)
else()
    target_sources(driver PRIVATE
        mock/io_base.h
        mock/io.cpp
    )
    target_include_directories(driver PRIVATE mock)
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

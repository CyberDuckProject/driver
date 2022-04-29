add_executable(driver
    msg/error.cpp
    msg/error.h
    msg/message_buffer.h
    msg/message.h
    msg/read.h
    msg/write.h
    net/signal_handler.h
    net/connection.h
    utl/allocate_from_token.h
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

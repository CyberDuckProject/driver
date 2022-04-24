add_executable(driver
    net/async_read_message.h
    net/connection_manager.h
    net/error.cpp
    net/error.h
    net/message_buffer.h
    net/messages.h
    net/signal_handler.h
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

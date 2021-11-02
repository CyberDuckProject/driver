add_library(Asio INTERFACE)

# third_party namespace
add_library(third_party::Asio ALIAS Asio)

# Include directory
target_include_directories(Asio INTERFACE include)

# Asio requires the system threading library
find_package(Threads REQUIRED)
target_link_libraries(Asio INTERFACE Threads::Threads)

add_library(pigpio STATIC
    include/pigpio.h
    src/command.h
    src/command.c
    src/custom.cext
    src/pigpio.c
)

# third_party namespace
add_library(third_party::pigpio ALIAS pigpio)

# Include directory
target_include_directories(pigpio PUBLIC include)

# Link against pthread
find_package(Threads REQUIRED)
target_link_libraries(pigpio PRIVATE Threads::Threads)

add_executable(driver
	main.cpp
	fundamental_types.h
	io.h
	io.cpp
	gpio_error.cpp
)

# Find dependencies
find_package(pigpio REQUIRED)

# Link against libraries
target_link_libraries(driver PRIVATE
	pigpio::pigpio
	third_party::Asio
)

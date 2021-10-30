add_executable(driver
	main.cpp
)

# Find dependencies
find_package(pigpio REQUIRED)
find_package(Boost REQUIRED COMPONENTS
	system thread
)

# Link against libraries
target_link_libraries(driver PRIVATE
	pigpio::pigpio
	Boost::system Boost::thread
)

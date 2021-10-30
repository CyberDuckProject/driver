add_executable(driver
	main.cpp
)

find_package(pigpio REQUIRED)
find_package(Boost REQUIRED COMPONENTS
	system thread
)

target_link_libraries(driver PRIVATE
	pigpio::pigpio
	Boost::system Boost::thread
)

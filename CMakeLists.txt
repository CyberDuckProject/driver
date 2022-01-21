cmake_minimum_required(VERSION 3.0)

# Project info
project(driver
    DESCRIPTION "CyberDuck driver"
    LANGUAGES C CXX
)

# Custom find modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party)
add_subdirectory(src)

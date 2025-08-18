cmake_minimum_required(VERSION 3.20)
project(daemonium LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3 (assumes it's installed on the system)
find_package(SDL3 REQUIRED)

add_executable(daemonium
    src/main.cpp
    src/Game.cpp
    src/Menu.cpp
)

target_include_directories(daemonium PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(daemonium PRIVATE ${SDL3_LIBRARIES})
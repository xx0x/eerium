cmake_minimum_required(VERSION 3.20)
project(daemonium LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 (assumes it's installed on the system)
find_package(SDL2 REQUIRED)

add_executable(daemonium
    src/main.cpp
    src/Game.cpp
    src/Menu.cpp
)

target_include_directories(daemonium PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(daemonium PRIVATE ${SDL2_LIBRARIES})
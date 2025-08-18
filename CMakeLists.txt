cmake_minimum_required(VERSION 3.20)
project(eerium LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3 (assumes it's installed on the system)
find_package(SDL3 REQUIRED)

add_executable(eerium
    src/main.cpp
    src/Game.cpp
    src/Menu.cpp
)

target_include_directories(eerium PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(eerium PRIVATE ${SDL3_LIBRARIES})
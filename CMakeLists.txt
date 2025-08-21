cmake_minimum_required(VERSION 3.20)
project(eerium LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3 (assumes it's installed on the system)
find_package(SDL3 REQUIRED)

# Use pkg-config for SDL3_ttf
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3_TTF REQUIRED sdl3-ttf)
pkg_check_modules(SDL3_IMAGE REQUIRED sdl3-image)

add_executable(eerium
    src/main.cpp
    src/Game.cpp
    src/MainMenu.cpp
    src/sdl/Font.cpp
    src/sdl/ResourceManager.cpp
    src/sdl/Window.cpp
    src/sdl/Renderer.cpp
    src/sdl/Context.cpp
    src/sdl/FpsCounter.cpp
)

target_include_directories(eerium PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${SDL3_INCLUDE_DIRS} 
    ${SDL3_TTF_INCLUDE_DIRS}
    ${SDL3_IMAGE_INCLUDE_DIRS}
)
target_link_libraries(eerium PRIVATE ${SDL3_LIBRARIES} ${SDL3_TTF_LIBRARIES} ${SDL3_IMAGE_LIBRARIES})
target_link_directories(eerium PRIVATE ${SDL3_TTF_LIBRARY_DIRS} ${SDL3_IMAGE_LIBRARY_DIRS})

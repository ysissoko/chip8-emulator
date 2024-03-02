cmake_minimum_required(VERSION 3.10)
project(Chip8 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find sfml library installed on filesystem
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options system)
# find_package(nlohmann_json 3.11.2 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})

# libs
list(APPEND EXTRA_LIBS spdlog::spdlog)
list(APPEND EXTRA_LIBS sfml-graphics)
list(APPEND EXTRA_LIBS Boost::program_options)

# Sources
list(APPEND SOURCES src/main.cpp)
list(APPEND SOURCES src/cpu/cpu.cpp)
list(APPEND SOURCES src/display/screen.cpp)
list(APPEND SOURCES src/input/keyboard.cpp)


add_executable(Chip8 ${SOURCES})

target_link_libraries(Chip8 PRIVATE ${EXTRA_LIBS})

target_include_directories(Chip8 PRIVATE "${PROJECT_SOURCE_DIR}")

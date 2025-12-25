# To build and run from the main directory (assuming Linux):
# - Create a `build/` dir if there isn't one already
# - Run: `cd build/ ; cmake ../src/ && make && ./VigilantWaddle`

############
# Preamble #
############

cmake_minimum_required(VERSION 3.28)
project(VigilantWaddle VERSION 0.1.0)

#################
# Project setup #
#################

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic-errors")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Og")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

################
# Dependencies #
################

# libtcod and sdl3 will need to be installed on your system.
find_package(SDL3 CONFIG REQUIRED) # Required by libtcod
find_package(libtcod CONFIG REQUIRED)

################
# Main targets #
################

# "Modules" (colloquially; not C++20 modules, which Make doesn't support yet, or
# at least "CMake with the Make generator" doesn't support yet) are defined here
# as CMake "object libraries" (even though they're not actually libraries).

set(OBJ_DIR "${PROJECT_SOURCE_DIR}/Object")
add_library(OBJ OBJECT)
target_sources(OBJ PRIVATE "${OBJ_DIR}/Object.cpp")
target_include_directories(OBJ PUBLIC "${OBJ_DIR}")

set(MAP_DIR "${PROJECT_SOURCE_DIR}/Map")
add_library(MAP OBJECT)
target_sources(MAP PRIVATE "${MAP_DIR}/Map.cpp")
target_include_directories(MAP PUBLIC "${MAP_DIR}")
target_link_libraries(MAP PRIVATE OBJ)

# Main executable
add_executable("${PROJECT_NAME}")
target_sources(
    "${PROJECT_NAME}" PRIVATE
    "${PROJECT_SOURCE_DIR}/main.cpp"
)
target_link_libraries(
    "${PROJECT_NAME}" PRIVATE
    MAP
    OBJ
)
target_link_libraries(
    "${PROJECT_NAME}" PRIVATE
    SDL3::SDL3
    libtcod::libtcod
)

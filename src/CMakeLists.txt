# Make sure SDL3 and its OPTIONAL EXTENSIONS (!) SDL3_image and SDL3_ttf are all
# installed on the system you're building this on. In Arch, that's the packages:
#     - `sdl3`
#     - `sdl3_ttf`
#     - `sdl3_image` (AUR package, unlike the TTF extension)
#
# To build and run this, from the main directory, do:
# ```
# mkdir build ; cd build/
# cmake ../src && make && ./vigilant-waddle
# ```
############
# Preamble #
############

cmake_minimum_required(VERSION 3.28)
project(vigilant-waddle VERSION 0.1.0)

#################
# Project setup #
#################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -Wall \
    -Wextra \
    -Werror \
    -Wfatal-errors \
    -pedantic-errors \
    -Wnull-dereference")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
    -g3 -Og -fsanitize=undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

################
# Dependencies #
################

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

################
# Main targets #
################

# "Modules" are defined here as CMake "object libraries" (even though they're
# not actually libraries). (They're also not C++20 modules, which Make doesn't
# support yet... or at least "CMake with the Make generator" doesn't support
# them yet.)
#
# The common directory is for modules used by other modules (in other words:
# it's for non-standalone modules).
set(COMMON_DIR "${PROJECT_SOURCE_DIR}/common/")

set(GAMEWINDOW_DIR "${COMMON_DIR}/GameWindow/")
add_library(GAMEWINDOW OBJECT)
target_sources(GAMEWINDOW PRIVATE "${GAMEWINDOW_DIR}/GameWindow.cpp")
target_include_directories(GAMEWINDOW PUBLIC "${GAMEWINDOW_DIR}")
target_include_directories(GAMEWINDOW PUBLIC "${COMMON_DIR}")

set(TEXTSERVICE_DIR "${COMMON_DIR}/TextService/")
add_library(TEXTSERVICE OBJECT)
target_sources(TEXTSERVICE PRIVATE "${TEXTSERVICE_DIR}/TextService.cpp")
target_include_directories(TEXTSERVICE PUBLIC "${TEXTSERVICE_DIR}")
target_link_libraries(TEXTSERVICE PRIVATE GAMEWINDOW)

set(PLAYSTAGE_DIR "${PROJECT_SOURCE_DIR}/PlayStage/")
add_library(PLAYSTAGE OBJECT)
target_sources(PLAYSTAGE PRIVATE "${PLAYSTAGE_DIR}/PlayStage.cpp")
target_sources(PLAYSTAGE PRIVATE "${PLAYSTAGE_DIR}/Grid.cpp")
target_sources(PLAYSTAGE PRIVATE "${PLAYSTAGE_DIR}/Entities.cpp")
target_include_directories(PLAYSTAGE PUBLIC "${PLAYSTAGE_DIR}")
target_include_directories(PLAYSTAGE PUBLIC "${COMMON_DIR}")
target_link_libraries(PLAYSTAGE PRIVATE GAMEWINDOW)
target_link_libraries(PLAYSTAGE PRIVATE TEXTSERVICE)

set(MENU_DIR "${PROJECT_SOURCE_DIR}/Menu/")
add_library(MENU OBJECT)
target_sources(MENU PRIVATE "${MENU_DIR}/Menu.cpp")
target_include_directories(MENU PUBLIC "${MENU_DIR}")
target_include_directories(MENU PUBLIC "${COMMON_DIR}")
target_link_libraries(MENU PRIVATE GAMEWINDOW)
target_link_libraries(MENU PRIVATE TEXTSERVICE)

# Main executable
add_executable("${PROJECT_NAME}")
target_sources("${PROJECT_NAME}" PRIVATE "${PROJECT_SOURCE_DIR}/main.cpp")
# Native modules
target_link_libraries("${PROJECT_NAME}" PRIVATE GAMEWINDOW)
target_link_libraries("${PROJECT_NAME}" PRIVATE TEXTSERVICE)
target_link_libraries("${PROJECT_NAME}" PRIVATE MENU)
target_link_libraries("${PROJECT_NAME}" PRIVATE PLAYSTAGE)
# Third-party libs
target_link_libraries("${PROJECT_NAME}" PRIVATE SDL3)
target_link_libraries("${PROJECT_NAME}" PRIVATE SDL3_image)
target_link_libraries("${PROJECT_NAME}" PRIVATE SDL3_ttf)

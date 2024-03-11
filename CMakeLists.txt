cmake_minimum_required(VERSION 3.26)
project(rgr_v2)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(ADDITIONAL_LIBRARIES panel)
add_executable(rgr_v2 main.cpp
        tui.cpp)
target_link_libraries(rgr_v2 ${CURSES_LIBRARIES})
target_link_libraries(rgr_v2 panel)

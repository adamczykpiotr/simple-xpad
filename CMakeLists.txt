cmake_minimum_required(VERSION 3.18)
project(simple-xpad)

set(CMAKE_CXX_STANDARD 14)

add_executable(simple-xpad
        main.cpp
        src/XPad/XPad.cpp
        src/XPad/XEvent.cpp)

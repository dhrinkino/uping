cmake_minimum_required(VERSION 3.1)
project(uping)

set(CMAKE_CXX_STANDARD 17)

# MACOS
link_directories(/opt/homebrew/lib)
include_directories(/opt/homebrew/Cellar/libtins/4.5/include)
include_directories(/opt/homebrew/Cellar/boost/1.88.0/include/boost)
include_directories(/opt/homebrew/include)


#include dir project
include_directories(include)

# select all src cpp files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(uping ${SOURCES}
        src/configure.cpp
        include/configure.h
        include/help.h
        src/help.cpp
        src/debug.cpp
        include/debug.h
        include/session.h)

# Link tins library
target_link_libraries(uping tins boost_system)

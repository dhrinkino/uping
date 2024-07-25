cmake_minimum_required(VERSION 3.1)
project(uping)

set(CMAKE_CXX_STANDARD 11)
include_directories(/opt/homebrew/Cellar/libtins/4.5/include)
link_directories(/opt/homebrew/lib)


add_executable(uping main.cpp
        icmpv6.cpp
        icmpv6.h
        generator.cpp
        generator.h
        udp6.cpp
        udp6.h
        tcp6.cpp
        tcp6.h
        tcp.cpp
        tcp.h
        icmp.cpp
        icmp.h
        udp.cpp
        udp.h
        randomIPv6.cpp
        randomIPv6.h
        randomIPv4.cpp
        randomIPv4.h
)

# MACOS brew locations
include_directories(/opt/homebrew/Cellar/libtins/4.5/include)
link_directories(/opt/homebrew/lib)


# Link tins to uping project
target_link_libraries(uping tins)

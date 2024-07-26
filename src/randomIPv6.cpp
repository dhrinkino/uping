

#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>

std::string randomIPv6() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dis;

    // force 2001
    const uint16_t firstChunk = 0x2001;

    uint16_t chunks[7];
    for (int i = 0; i < 7; ++i) {
        chunks[i] = dis(gen);
    }

    std::stringstream ss;
    ss << std::hex << std::setw(4) << std::setfill('0') << firstChunk;
    for (int i = 0; i < 7; ++i) {
        ss << ":" << std::hex << std::setw(4) << std::setfill('0') << chunks[i];
    }

    return ss.str();
}
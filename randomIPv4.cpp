#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>

std::string randomIPv4() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255);

    uint8_t octets[4];
    for (int i = 0; i < 4; ++i) {
        octets[i] = dis(gen);
    }

    // merge together
    std::stringstream ss;
    ss << static_cast<int>(octets[0]);
    for (int i = 1; i < 4; ++i) {
        ss << "." << static_cast<int>(octets[i]);
    }

    return ss.str();
}
#include <iostream>
#include <sstream>
#include <random>

std::string randomIPv4() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<uint8_t> firstOctetRanges;

    // push to one vector all possible first octets
    for (uint8_t i = 1; i <= 9; ++i) {
        firstOctetRanges.push_back(i);
    }
    for (uint8_t i = 11; i <= 126; ++i) {
        firstOctetRanges.push_back(i);
    }
    for (uint8_t i = 128; i <= 168; ++i) {
        firstOctetRanges.push_back(i);
    }
    for (uint8_t i = 173; i <= 191; ++i) {
        firstOctetRanges.push_back(i);
    }
    for (uint8_t i = 193; i <= 223; ++i) {
        firstOctetRanges.push_back(i);
    }

    std::uniform_int_distribution<size_t> firstOctetDist(0, firstOctetRanges.size() - 1);
    uint8_t firstOctet = firstOctetRanges[firstOctetDist(gen)];

    std::uniform_int_distribution<uint8_t> dis(0, 255);
    uint8_t octets[4];
    octets[0] = firstOctet; // force first octet
    for (int i = 1; i < 4; ++i) {
        octets[i] = dis(gen);
    }

    std::stringstream ss;
    ss << static_cast<int>(octets[0]);
    for (int i = 1; i < 4; ++i) {
        ss << "." << static_cast<int>(octets[i]);
    }

    return ss.str();
}
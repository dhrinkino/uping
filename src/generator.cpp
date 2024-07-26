#include <string>
//
// Created by Dominik Hrinkino on 25/07/2024.
//
using namespace std;


string generate(int size) {
    if (size == 0) {
        return "";
    }
    std::string str;
    str.reserve(size);
    for (size_t i = 0; i < size - 1; ++i) {
        str += 'a';
    }
    // finalize string
    str += '\0';
    return str;
}
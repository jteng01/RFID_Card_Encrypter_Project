#ifndef BYTEMANIP_H
#define BYTEMANIP_H

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>

std::string decToHexString(int decimalValue) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << decimalValue;
    return ss.str();
}

std::string reverseBytesPerBlock(const std::string s) {

    std::string result;
    result.reserve(s.length());

    for (size_t i = 0; i < s.length(); i += 8) {
        const std::string block = s.substr(i, 8);

        result += block.substr(6, 2);
        result += block.substr(4, 2);
        result += block.substr(2, 2);
        result += block.substr(0, 2);
    }

    return result;
}

std::string reverseBytes(const std::string s) {

    std::string result;
    result.reserve(s.length());

    for (int i = s.length() - 2; i >= 0; i -= 2) {
        result += s.substr(i, 2);
    }

    return result;
}

#endif
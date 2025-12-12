#include "bytemanip.h"
#include <sstream>
#include <iomanip>


std::string decToHexString(int decimalValue) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << decimalValue;
    return ss.str();
}

std::string reverseBytesPerBlock(const std::string& s) {
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

std::string reverseBytes(const std::string& s) {
    std::string result;
    result.reserve(s.length());

    for (int i = static_cast<int>(s.length()) - 2; i >= 0; i -= 2) {
        result += s.substr(i, 2);
    }

    return result;
}

std::string asciiToHexString(const std::string& input) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');

    for (unsigned char c : input) {
        oss << std::setw(2) << static_cast<int>(c);
    }
    return oss.str();
}

std::string hexStringToAscii(const std::string& hex) {
    std::string output;

    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteStr = hex.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteStr, nullptr, 16));
        output.push_back(byte);
    }

    return output;
}

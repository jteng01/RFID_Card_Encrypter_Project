#ifndef BYTEMANIP_H
#define BYTEMANIP_H

#include <string>

std::string decToHexString(int decimalValue);
std::string reverseBytesPerBlock(const std::string& s);
std::string reverseBytes(const std::string& s);
std::string asciiToHexString(const std::string& input);
std::string hexStringToAscii(const std::string& hex);

#endif
#include "reader.h"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include "serial.h"
#include "bytemanip.h"
#include <string>

namespace RFID {

    RFIDReader::RFIDReader(const std::string& port, DWORD baudRate)
        : serial(std::make_unique<SerialPort>(port.c_str(), baudRate)), uid("")
    {
    }

    void RFIDReader::resetPort(const std::string& port, DWORD baudRate)
    {
        serial.reset();
        serial = std::make_unique<SerialPort>(port.c_str(), baudRate);
        uid.clear();
    }

    std::string RFIDReader::readRawDataFromBlockAddress(int block) {
        std::string blockIndex = decToHexString(block);
        std::string command = OPCODE_READBLOCK_ADDRESS + uid + blockIndex + CLOSE_RWBLOCK;
        serial->sendData(command.c_str());
        return serial->receiveData();
    }

    std::string RFIDReader::writeHexDataToBlockAddress(int block, std::string blockData) {
        std::string blockIndex = decToHexString(block);
        std::string command = OPCODE_WRITEBLOCK_ADDRESS + uid + blockIndex + reverseBytes(blockData) + CLOSE_RWBLOCK;
        serial->sendData(command.c_str());
        return serial->receiveData();
    }

    std::string RFIDReader::readRawDataFromBlock(int block) {
        std::string blockIndex = decToHexString(block);
        std::string command = OPCODE_READBLOCK_ADDRESS + blockIndex + CLOSE_RWBLOCK;
        serial->sendData(command.c_str());
        return serial->receiveData();
    }

    std::string RFIDReader::writeHexDataToBlock(int block, std::string blockData) {
        std::string blockIndex = decToHexString(block);
        std::string command = OPCODE_WRITEBLOCK_ADDRESS + blockIndex + reverseBytes(blockData) + CLOSE_RWBLOCK;
        serial->sendData(command.c_str());
        return serial->receiveData();
    }


    bool RFIDReader::checkValidBlock(int block) {
        std::string response = readRawDataFromBlockAddress(block);
        return (response.find("[0110]") == std::string::npos);
    }


    void RFIDReader::setFullPower() {
        serial->sendData("0108000304FC0000"); serial->receiveData();
        serial->sendData("010C00030410002101000000"); serial->receiveData();
        serial->sendData("0109000304F0000000"); serial->receiveData();
        serial->sendData("0109000304F1FF0000"); serial->receiveData();
    }

    void RFIDReader::setHalfPower() {
        serial->sendData("0108000304FC0000"); serial->receiveData();
        serial->sendData("010C00030410003101000000"); serial->receiveData();
        serial->sendData("0109000304F0000000"); serial->receiveData();
        serial->sendData("0109000304F1FF0000"); serial->receiveData();
    }

    std::string RFIDReader::readUID() {
        serial->sendData("010B000304142401000000");
        std::string response = serial->receiveData();
        if (response.find("7F") == std::string::npos || response.find("z") != std::string::npos) {
            return "";
        }
        int indexStart = response.find("[") + 1;
        uid = response.substr(indexStart, 16);
        return uid;
    }

    std::string RFIDReader::getUID() {
        return uid;
    }

    void RFIDReader::clearUID() {
        uid.clear();
    }


    std::string RFIDReader::readDataFromCardAddress() {
        std::string data;
        for (int i = 0; i < 0xFF; i++) {
            std::string response;
            bool success = false;
            for (int attempt = 0; attempt < 3; attempt++) {
                response = readRawDataFromBlockAddress(i);
                if (response.find("[]") == std::string::npos) { success = true; break; }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (!success) return "";
            if (response.find("[z]") != std::string::npos ||
                response.find("Interrupt error") != std::string::npos ||
                response.find("130B") == std::string::npos) return "";
            if (response.find("[0110]") != std::string::npos) break;

            size_t start = response.find('[');
            size_t end = response.find(']');
            if (start == std::string::npos || end == std::string::npos || end <= start) continue;

            std::string payload = response.substr(start + 1, end - start - 1);
            if (payload.size() >= 2 && payload.substr(0, 2) == "00") {
                data += payload.substr(2);
            }
        }
        return reverseBytesPerBlock(data);
    }

    std::string RFIDReader::readAsciiDataFromCardAddress() {
        return hexStringToAscii(readDataFromCardAddress());
    }

    void RFIDReader::writeHexDataToCardAddress(std::string data) {
        int remainder = data.size() % 8;
        if (remainder != 0) data += std::string(8 - remainder, '0');

        const int TIMEOUT_MS = 3000;
        const int RETRY_DELAY_MS = 30;

        for (int i = 0; i <= 0xff; i++) {
            if (!checkValidBlock(i)) break;
            std::string blockData = (i * 8 >= data.size()) ? "00000000" : data.substr(i * 8, 8);
            std::string command = OPCODE_WRITEBLOCK_ADDRESS + uid + decToHexString(i) + reverseBytes(blockData) + CLOSE_RWBLOCK;
            serial->sendData(command.c_str());
            std::string response = serial->receiveData();
            auto start = std::chrono::steady_clock::now();

            while (response.find("[00]") == std::string::npos) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
                if (elapsed >= TIMEOUT_MS) {
                    throw std::runtime_error("Failed to write to card. Request timed out.");
                    return;
                }
                if (response.find("[0112]") != std::string::npos) {
                    return;
                }
                serial->sendData(command.c_str());
                response = serial->receiveData();
                std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_DELAY_MS));
            }
        }
    }

    void RFIDReader::writeAsciiDataToCardAddress(std::string asciiData) {
        writeHexDataToCardAddress(asciiToHexString(asciiData));
    }


    std::string RFIDReader::readRawDataFromCard() {
        std::string data;
        for (int i = 0; i < 0xFF; i++) {
            std::string response;
            bool success = false;
            for (int attempt = 0; attempt < 3; attempt++) {
                response = readRawDataFromBlock(i);
                if (response.find("[]") == std::string::npos) { success = true; break; }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (!success) return "";
            if (response.find("[z]") != std::string::npos ||
                response.find("Interrupt error") != std::string::npos ||
                response.find("130B") == std::string::npos) return "";
            if (response.find("[0110]") != std::string::npos) break;

            size_t start = response.find('[');
            size_t end = response.find(']');
            if (start == std::string::npos || end == std::string::npos || end <= start) continue;

            std::string payload = response.substr(start + 1, end - start - 1);
            if (payload.size() >= 2 && payload.substr(0, 2) == "00") {
                data += payload.substr(2);
            }
        }
        return reverseBytesPerBlock(data);
    }

    std::string RFIDReader::readAsciiDataFromCard() {
        return hexStringToAscii(readRawDataFromCard());
    }

    void RFIDReader::writeHexDataToCard(std::string data) {
        int remainder = data.size() % 8;
        if (remainder != 0) data += std::string(8 - remainder, '0');

        const int TIMEOUT_MS = 5000;
        const int RETRY_DELAY_MS = 30;

        for (int i = 0; i <= 0xff; i++) {
            if (!checkValidBlock(i)) break;
            std::string blockData = (i * 8 >= data.size()) ? "00000000" : data.substr(i * 8, 8);
            std::string command = OPCODE_WRITEBLOCK_ADDRESS + decToHexString(i) + reverseBytes(blockData) + CLOSE_RWBLOCK;
            serial->sendData(command.c_str());
            std::string response = serial->receiveData();
            auto start = std::chrono::steady_clock::now();

            while (response.find("[00]") == std::string::npos) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
                if (elapsed >= TIMEOUT_MS) {
                    std::cout << "Failed to write to card. Request timed out." << std::endl;
                    return;
                }
                if (response.find("[0112]") != std::string::npos) {
                    std::cout << "End of writable memory at block " << i << std::endl;
                    return;
                }
                serial->sendData(command.c_str());
                response = serial->receiveData();
                std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_DELAY_MS));
            }
        }
    }

    void RFIDReader::writeAsciiDataToCard(std::string asciiData) {
        writeHexDataToCard(asciiToHexString(asciiData));
    }

}

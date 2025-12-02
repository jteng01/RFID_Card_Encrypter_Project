#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include "serial.h"
#include "bytemanip.h"

#define OPCODE_READBLOCK "010B000304180020"
#define OPCODE_WRITEBLOCK "010F000304184021"
#define OPCODE_WRITEBLOCK_ADDRESS "0117000304186021"
#define OPCODE_READBLOCK_ADDRESS "0113000304182020"
#define CLOSE_RWBLOCK "0000"

class RFIDReader {
private:
    SerialPort serial;
    std::string uid;

public:
    RFIDReader(const char* port, DWORD baudRate) : serial(port, baudRate), uid("") {}

    std::string readRawDataFromBlockAddress(int block) {
        std::string blockIndex = decToHexString(block);
        std::string command = OPCODE_READBLOCK_ADDRESS + uid + blockIndex + CLOSE_RWBLOCK;
        std::cout << command << std::endl;
        serial.sendData(command.c_str());
        std::string response = serial.receiveData();
        std::cout << response << std::endl;
        return response;
    }

    bool checkValidBlock(int block) {
        std::string response = readRawDataFromBlockAddress(block);
        return (response.find("[0110]") == std::string::npos || response.find("[]") == std::string::npos);
    }

    void setFullPower() {
        serial.sendData("0108000304FC0000");
        serial.receiveData();
        serial.sendData("010C00030410002101000000");
        serial.receiveData();
        serial.sendData("0109000304F0000000");
        serial.receiveData();
        serial.sendData("0109000304F1FF0000");
        serial.receiveData();
    }

    void setHalfPower() {
        serial.sendData("0108000304FC0000");
        serial.receiveData();
        serial.sendData("010C00030410003101000000");
        serial.receiveData();
        serial.sendData("0109000304F0000000");
        serial.receiveData();
        serial.sendData("0109000304F1FF0000");
        serial.receiveData();
    }

    std::string getUID() {
        serial.sendData("010B000304142401000000");
        std::string response = serial.receiveData();
        std::cout << response << std::endl;
        int indexStart = response.find("[") + 1;
        uid = response.substr(indexStart, 16);
        this->uid = uid;
        return uid;
    }

    std::string readDataFromCardAddress() {
        std::string data;
        for (int i = 0; i < 0xff; i++) {
            std::string response = readRawDataFromBlockAddress(i);

            if (response.find("[0110]") != std::string::npos) {
                std::cout << "End of data reached at block: " << i << std::endl;
                break;
            }

            size_t start = response.find('[') + 1;
            size_t end   = response.find(']');
            if (start == std::string::npos || end == std::string::npos || end <= start)
                continue;

            std::string payload = response.substr(start, end - start);

            if (payload.size() >= 2 && payload.substr(0, 2) == "00") {
                data += payload.substr(2); 
            }
        }
        return data;
    }

    void writeHexDataToCardAddress(std::string data) {
        int remainder = data.size() % 8;
        if (remainder != 0) {
            data += std::string(8 - remainder, '0');
        }

        for (int i = 0; i <= 0xff; i++) {
            if (!checkValidBlock(i)) break;
            std::string blockIndex = decToHexString(i);
            std::string blockData = (i * 8 >= data.size()) ? "00000000" : data.substr(i * 8, 8);
            std::string command = OPCODE_WRITEBLOCK_ADDRESS + uid + blockIndex + blockData + CLOSE_RWBLOCK;
            serial.sendData(command.c_str());
            serial.receiveData();
        }
    }
};


int main() {
    RFIDReader reader("COM7", 115200);
    
    reader.setFullPower();

    std::string uid = reader.getUID();
    std::cout << "Card UID: " << uid << std::endl;

    std::string cardData = reader.readDataFromCardAddress();
    std::cout << "Card data: " << cardData << std::endl;


    /*
    std::string testString = "";
    for (int i = 0; i < 10; i++)
    {   
        std::string hexDigit = decToHexString(i);
        std::string genString(8, hexDigit[1]);
        testString += genString;
    }

    reader.writeHexDataToCardAddress(testString);
    std::cout << "Wrote data to card." << std::endl;
    */

    cardData = reader.readDataFromCardAddress();
    std::cout << "Card data: " << cardData << std::endl;


    return 0;
}
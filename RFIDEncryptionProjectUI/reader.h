#ifndef READER_H
#define READER_H

#include "serial.h"
#include <string>
#include <memory>

#define OPCODE_READBLOCK "010B000304180020"
#define OPCODE_WRITEBLOCK "010F000304184021"
#define OPCODE_WRITEBLOCK_ADDRESS "0117000304186021"
#define OPCODE_READBLOCK_ADDRESS "0113000304182020"
#define CLOSE_RWBLOCK "0000"

namespace RFID {

    class RFIDReader {

    private:
        std::unique_ptr<SerialPort> serial;
        std::string uid;

    public:
        RFIDReader(const std::string& port, DWORD baudRate);

        void resetPort(const std::string& port, DWORD baudRate);

        std::string readRawDataFromBlockAddress(int block);
        std::string writeHexDataToBlockAddress(int block, std::string blockData);
        std::string readRawDataFromBlock(int block);
        std::string writeHexDataToBlock(int block, std::string blockData);

        bool checkValidBlock(int block);
        void setFullPower();
        void setHalfPower();

        std::string readUID();
        std::string getUID();
        void clearUID();

        std::string readDataFromCardAddress();
        std::string readAsciiDataFromCardAddress();
        void writeHexDataToCardAddress(std::string data);
        void writeAsciiDataToCardAddress(std::string asciiData);

        std::string readRawDataFromCard();
        std::string readAsciiDataFromCard();
        void writeHexDataToCard(std::string data);
        void writeAsciiDataToCard(std::string asciiData);
    };

} // namespace RFID

#endif // READER_H

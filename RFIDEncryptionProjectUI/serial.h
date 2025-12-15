#ifndef SERIAL_H
#define SERIAL_H

#include <windows.h>
#include <string>

class SerialPort {
private:
    HANDLE hSerial;
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;

public:
    SerialPort(const std::string& portName, DWORD baudRate);
    ~SerialPort();

    void sendData(const char* data);
    std::string receiveData();
    void changePort(const char* portName);
};

#endif

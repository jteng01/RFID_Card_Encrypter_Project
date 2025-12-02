#ifndef SERIAL_H
#define SERIAL_H

#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>

class SerialPort {
private:
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

public:
    SerialPort(const char* portName, DWORD baudRate) {
        hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to open serial port" << std::endl;
            return;
        }

        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        GetCommState(hSerial, &dcbSerialParams);
        dcbSerialParams.BaudRate = baudRate;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        SetCommState(hSerial, &dcbSerialParams);

        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;
        SetCommTimeouts(hSerial, &timeouts);
    }

    void sendData(const char* data) {
        DWORD bytesWritten;
        WriteFile(hSerial, data, strlen(data), &bytesWritten, NULL);
    }

    std::string receiveData() {
        char buffer[256] = {0};
        DWORD bytesRead;
        ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        return std::string(buffer);
    }

    void changePort(const char* portName) {
        if (hSerial != INVALID_HANDLE_VALUE) {
            CloseHandle(hSerial);
        }
        hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to open serial port: " << portName << std::endl;
        }
    }

    ~SerialPort() {
        if (hSerial != INVALID_HANDLE_VALUE) {
            CloseHandle(hSerial);
        }
    }
};

#endif

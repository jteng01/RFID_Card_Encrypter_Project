#include "serial.h"
#include <iostream>
#include <cstring>
#include <windows.h>
#include <string>


SerialPort::SerialPort(const char* portName, DWORD baudRate) {
    hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open serial port: " << portName << std::endl;
        return;
    }

    dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);
}

SerialPort::~SerialPort() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
}

void SerialPort::sendData(const char* data) {
    DWORD bytesWritten;
    WriteFile(hSerial, data, strlen(data), &bytesWritten, NULL);
}

std::string SerialPort::receiveData() {
    char buffer[256] = { 0 };
    DWORD bytesRead;
    ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
    return std::string(buffer);
}

void SerialPort::changePort(const char* portName) {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }

    hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open serial port: " << portName << std::endl;
    }
}


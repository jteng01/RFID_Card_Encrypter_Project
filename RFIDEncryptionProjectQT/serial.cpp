#include "serial.h"
#include <stdexcept>
#include <windows.h>
#include <string>

SerialPort::SerialPort(const std::string& portName, DWORD baudRate)
{
    hSerial = CreateFileA(
        portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );

    if (hSerial == INVALID_HANDLE_VALUE) {
        throw std::runtime_error(
            std::string("Failed to open serial port: ") + portName
            );
    }

    DCB dcbSerialParams{};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        throw std::runtime_error("GetCommState failed");
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        throw std::runtime_error("SetCommState failed");
    }

    COMMTIMEOUTS timeouts{};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        CloseHandle(hSerial);
        throw std::runtime_error("SetCommTimeouts failed");
    }
}


SerialPort::~SerialPort() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
}

void SerialPort::sendData(const char* data)
{
    DWORD totalWritten = 0;
    DWORD length = static_cast<DWORD>(strlen(data));

    while (totalWritten < length) {
        DWORD written = 0;

        if (!WriteFile(
                hSerial,
                data + totalWritten,
                length - totalWritten,
                &written,
                NULL)) {
            throw std::runtime_error("Serial write failed");
        }

        totalWritten += written;
    }
}


std::string SerialPort::receiveData()
{
    char buffer[256];
    DWORD bytesRead = 0;

    if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
        throw std::runtime_error("Serial read failed");
    }

    return std::string(buffer, bytesRead);
}

# RFID Card Encrypter Project

A small toolset for reading and encrypting data from RFID cards. This repository contains a native C++ command-line/GUI application and supporting libraries for byte manipulation, serial communication, and simple encryption routines used to work with an RFID reader.

**Status:** Development / proof-of-concept

**Requirements**
- **Hardware:** ISO15693 RFID reader with serial interface
- **Software:** 
  - Visual Studio 2019/2022 (for native C++ build), or
  - Qt Creator (for Qt-based UI build)
  - Windows OS (x64)

**Key components**
- `encryptor.cpp`, `encryptor.h`: high-level encryption logic.
- `bytemanip.cpp`, `bytemanip.h`: helper functions for byte/memory manipulation.
- `reader.cpp`, `reader.h`: RFID reader interface code.
- `serial.cpp`, `serial.h`: serial-port communication helpers.
- `main.cpp`: application entry for the console/GUI builds.
- `RFIDEncryptionProjectUI/`: Qt-based UI project and sources.

**Repository layout**
- [RFID_Card_Encrypter_Project.slnx](RFID_Card_Encrypter_Project.slnx) — Visual Studio solution for the native build.
- `x64/Debug/` — Visual Studio build outputs (when built in Debug x64).
- `RFIDEncryptionProjectUI/` — Qt Creator project and UI sources (desktop UI build using Qt).

**Build (Windows / Visual Studio)**
1. Open the solution file: [RFID_Card_Encrypter_Project.slnx](RFID_Card_Encrypter_Project.slnx) in Visual Studio 2019/2022.
2. Select the `x64` platform and `Debug` or `Release` configuration.
3. Build the solution (Build → Build Solution).
4. The executable is produced under `x64/Debug/` or `x64/Release/` depending on configuration.

**Build (Qt UI)**
1. Open `RFIDEncryptionProjectUI/RFIDEncryptionProject.pro` in Qt Creator.
2. Configure with your chosen Qt kit (e.g., MinGW or MSVC).
3. Run qmake / Configure and build from Qt Creator. The generated UI header is in `build/` when built.

**Running the application**
- From Visual Studio build output folder: run the produced executable (for example `x64/Debug/RFID_Card_Encrypter_Project.exe`).
- For the Qt UI, run the built UI binary from Qt Creator or the `build/` output folder.

General usage notes:
- Ensure the RFID reader is connected to the expected serial port.
- Configure any serial-port settings required by your reader (baud rate, parity, etc.) in the code or UI.
- Use the UI to read card data and invoke the encryption routines, or run the console build if a headless flow is implemented.

## Features
- Encrypt RFID card information
- Secure data storage
- Easy-to-use interface
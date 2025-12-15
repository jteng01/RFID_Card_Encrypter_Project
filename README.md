# RFID Card Encrypter Project
# RFID Card Encrypter Project

A small toolset for reading and encrypting data from RFID cards. This repository contains a native C++ command-line/GUI application and supporting libraries for byte manipulation, serial communication, and simple encryption routines used to work with an RFID reader.

**Status:** Development / proof-of-concept

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

**Developer notes**
- The project is intentionally small and focused. Look at `reader.cpp` / `serial.cpp` for hardware interface code and `encryptor.cpp` for encryption logic.
- If adding new hardware support, keep serial interactions isolated in `serial.*`.
- Follow the existing style for function naming and avoid global state where possible.

**Contributing**
- Fork the repo, create a feature branch, and open a pull request. Describe hardware used and reproduction steps for fixes.

**License**
This project is provided under the MIT License. See the LICENSE file (add one if needed).

**Contact**
If you need help building or adapting the project to a different reader, open an issue with details about your platform and reader model.

## Overview
A project for encrypting and managing RFID card data securely.

## Features
- Encrypt RFID card information
- Secure data storage
- Easy-to-use interface

## Installation
```bash
git clone https://github.com/yourusername/RFID_Card_Encrypter_Project.git
cd RFID_Card_Encrypter_Project
```

## Usage
```bash
python main.py
```

## Requirements
- Python 3.8+
- [List your dependencies]

## Contributing
Pull requests are welcome. For major changes, please open an issue first.

## License
[Choose appropriate license]

## Contact
For questions, reach out via GitHub Issues.
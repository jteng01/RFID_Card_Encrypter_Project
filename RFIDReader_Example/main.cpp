#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include "serial.h"
#include "bytemanip.h"
#include "reader.h"


int main() {
    RFIDReader reader("COM7", 115200);
    
    reader.setFullPower();

    
    while (reader.getUID() == "")
    {
        std::string uid = reader.readUID();
        Sleep(100);
        if (uid != "") {
            std::cout << "Card UID: " << reverseBytes(uid) << std::endl;
            
            try
            {
                reader.writeAsciiDataToCardAddress("0123ABCDE");
                std::string cardData = reader.readAsciiDataFromCardAddress();
                if (cardData == "") {
                    throw std::runtime_error("Error reading data from card.");
                }
                std::cout << cardData << std::endl;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            reader.clearUID();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        
    }
    
    


    return 0;
}
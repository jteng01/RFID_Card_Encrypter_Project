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
#include "encryptor.h"


void readAndWriteTest(){
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
}

void singleReadTest(){
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
                std::string cardData = reader.readAsciiDataFromCardAddress();
                if (cardData == "") {
                    throw std::runtime_error("Error reading data from card.");
                }
                std::cout << "Card Data: "<< cardData << std::endl;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            reader.clearUID();
            break;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        
    }
}

void encryptionTest(){
    try{
        Encryptor encryptor(new AESEncryption(), (unsigned char *)"0123456789ABCDEF0123456789ABCDEF", (unsigned char *)"ABCDEF0123456789");
        std::string plaintext = "Hello, World!";
        std::string encrypted = encryptor.encrypt(plaintext);
        std::cout << "Encrypted: " << encrypted << std::endl;
        encryptor.setIv((std::string)"ABCDEF0123456789");
        std::string decrypted = encryptor.decrypt(encrypted);
        std::cout << "Decrypted: " << decrypted << std::endl;
    }
    catch(const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void encryptCardTest(){
    RFIDReader reader("COM7", 115200);
    reader.setFullPower();
    Encryptor encryptor(new AESEncryption(), (unsigned char *)"0123456789ABCDEF0123456789ABCDEF", (unsigned char *)"ABCDEF0123456789");

    while (reader.getUID() == "")
    {
        std::string uid = reader.readUID();
        Sleep(100);
        if (uid != "") {
            std::cout << "Card UID: " << reverseBytes(uid) << std::endl;
            
            try
            {
                std::string dataToWrite = "SensitiveInfo";
                dataToWrite = encryptor.encrypt(dataToWrite);
                reader.writeAsciiDataToCardAddress(dataToWrite);
                std::cout << dataToWrite << std::endl;

            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            

            break;

            reader.clearUID();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        
    }

}

void decryptCardTest() {
    RFIDReader reader("COM7", 115200);
    reader.setFullPower();

    Encryptor encryptor(new AESEncryption(), (unsigned char *)"0123456789ABCDEF0123456789ABCDEF", (unsigned char *)"ABCDEF0123456789");

    while (reader.getUID() == "") {
        std::string uid = reader.readUID();
        Sleep(100);

        if (uid != "") {
            std::cout << "Card UID: " << reverseBytes(uid) << std::endl;

            try {
                std::string encryptedData = reader.readAsciiDataFromCardAddress();
                if (encryptedData == "") {
                    throw std::runtime_error("Error reading data from card.");
                }

                encryptor.setIv((std::string)"ABCDEF0123456789");
                std::string decryptedData = encryptor.decrypt(encryptedData);

                std::cout << "Decrypted Card Data: " << decryptedData << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }

            reader.clearUID();
            break;
        }
    }
}



int main() {
    encryptCardTest();
    singleReadTest();
    decryptCardTest();
    
    return 0;
}
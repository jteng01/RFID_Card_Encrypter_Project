#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <openssl/aes.h>


class EncryptionAlgorithm {
public:
    virtual ~EncryptionAlgorithm() = default;
    virtual std::string encrypt(const std::string& data, const std::string& key) = 0;
    virtual std::string decrypt(const std::string& data, const std::string& key) = 0;      
};



class AESEncryption : public EncryptionAlgorithm {
public:
    std::string encrypt(const std::string& data, const std::string& key) override {
        return "";
    }

    std::string decrypt(const std::string& data, const std::string& key) override {
        return "";
    }
};



class Encryptor {
private:
    EncryptionAlgorithm* encryptionAlgorithm;
    std::string key;

public:
    Encryptor(EncryptionAlgorithm* algorithm, const std::string& key) 
        : encryptionAlgorithm(algorithm), key(key) 
    {
        if (!algorithm) throw std::invalid_argument("Algorithm cannot be null");
    }

    void setKey(const std::string& key){
        this->key = key;
    }

    void setAlgorithm(EncryptionAlgorithm* algorithm){
        if (!algorithm) throw std::invalid_argument("Algorithm cannot be null");
        this->encryptionAlgorithm = algorithm;
    }

    std::string encrypt(const std::string& data){
        return encryptionAlgorithm->encrypt(data, key);
    }

    std::string decrypt(const std::string& data){
        return encryptionAlgorithm->decrypt(data, key);
    }
};

#endif

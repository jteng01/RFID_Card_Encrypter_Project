#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <openssl/evp.h>
#include <openssl/err.h>


class EncryptionAlgorithm {
public:
    virtual ~EncryptionAlgorithm() = default;
    virtual std::string encrypt(std::string data, unsigned char key[], unsigned char iv[]) = 0;
    virtual std::string decrypt(std::string data, unsigned char key[], unsigned char iv[]) = 0;      
};


class AESEncryption : public EncryptionAlgorithm {
private:
    void handleErrors() {
        ERR_print_errors_fp(stderr);
        ERR_clear_error();
        throw std::runtime_error("Encryption/Decryption error occurred");
    }

public:
    std::string encrypt(std::string data, unsigned char key[], unsigned char iv[]) override {
        if (data.size() > 32)
            data = data.substr(0, 32);

        unsigned char ciphertext[32];
        int outlen = 0, tmplen = 0;

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) handleErrors();

        if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
            handleErrors();

        if (1 != EVP_EncryptUpdate(ctx, ciphertext, &outlen,
                                   reinterpret_cast<const unsigned char*>(data.c_str()),
                                   data.length()))
            handleErrors();

        if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + outlen, &tmplen))
            handleErrors();

        int total = outlen + tmplen;

        EVP_CIPHER_CTX_free(ctx);

        return std::string(reinterpret_cast<char*>(ciphertext), total);
    }

    std::string decrypt(std::string data, unsigned char key[], unsigned char iv[]) override {
        if (data.size() > 32)
            data = data.substr(0, 32);

        unsigned char plaintext[32];
        int outlen = 0, tmplen = 0;

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) handleErrors();

        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
            handleErrors();

        if (1 != EVP_DecryptUpdate(ctx, plaintext, &outlen,
                                reinterpret_cast<const unsigned char*>(data.c_str()),
                                data.length()))
            handleErrors();

        if (1 != EVP_DecryptFinal_ex(ctx, plaintext + outlen, &tmplen))
            handleErrors();

        int total = outlen + tmplen;

        EVP_CIPHER_CTX_free(ctx);

        return std::string(reinterpret_cast<char*>(plaintext), total);
    }

};


class Encryptor {
private:
    EncryptionAlgorithm* encryptionAlgorithm;
    unsigned char key[32];
    unsigned char iv[16];

public:
    Encryptor(EncryptionAlgorithm* algorithm, unsigned char keyInput[32], const unsigned char ivInput[16]) 
        : encryptionAlgorithm(algorithm)
    {
        if (!algorithm) throw std::invalid_argument("Algorithm cannot be null");
        std::memcpy(this->key, keyInput, 32);
        std::memcpy(this->iv, ivInput, 16);
    }

    void setKey(const std::string& newKey) {
        memset(this->key, 0, 32);

        size_t len = newKey.size() < 32 ? newKey.size() : 32;
        std::copy(newKey.begin(), newKey.begin() + len, this->key);
    }

    void setIv(const std::string& newIv) {
        memset(this->iv, 0, 16);

        size_t len = newIv.size() < 16 ? newIv.size() : 16;
        std::copy(newIv.begin(), newIv.begin() + len, this->iv);
    }


    void setAlgorithm(EncryptionAlgorithm* algorithm){
        if (!algorithm) throw std::invalid_argument("Algorithm cannot be null");
        this->encryptionAlgorithm = algorithm;
    }

    std::string encrypt(const std::string& data){
        return encryptionAlgorithm->encrypt(data, this->key, this->iv);
    }

    std::string decrypt(const std::string& data){
        return encryptionAlgorithm->decrypt(data, this->key, this->iv);
    }
};

#endif

#include "encryptor.h"
#include <algorithm>
#include <iostream>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/err.h>

namespace Crypto {

    void AESEncryption::handleErrors() {
        ERR_print_errors_fp(stderr);
        ERR_clear_error();
        throw std::runtime_error("Encryption/Decryption error occurred");
    }


    std::string AESEncryption::encrypt(const std::string& data, unsigned char key[], unsigned char iv[]) {
        std::string input = data;
        if (input.size() > 32)
            input = input.substr(0, 32);

        unsigned char buffer[32] = { 0 };
        memcpy(buffer, input.data(), input.size());

        unsigned char ciphertext[32];
        int outlen = 0, tmplen = 0;

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) handleErrors();

        if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), nullptr, key, iv))
            handleErrors();

        if (1 != EVP_EncryptUpdate(ctx, ciphertext, &outlen, buffer, 32))
            handleErrors();

        EVP_EncryptFinal_ex(ctx, ciphertext + outlen, &tmplen);

        EVP_CIPHER_CTX_free(ctx);

        return std::string(reinterpret_cast<char*>(ciphertext), 32);
    }

    std::string AESEncryption::decrypt(const std::string& data, unsigned char key[], unsigned char iv[]) {
        std::string input = data;
        if (input.size() > 32)
            input = input.substr(0, 32);

        unsigned char plaintext[32];
        int outlen = 0, tmplen = 0;

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) handleErrors();

        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), nullptr, key, iv))
            handleErrors();

        if (1 != EVP_DecryptUpdate(ctx, plaintext, &outlen,
            reinterpret_cast<const unsigned char*>(input.data()), 32))
            handleErrors();

        EVP_DecryptFinal_ex(ctx, plaintext + outlen, &tmplen);

        EVP_CIPHER_CTX_free(ctx);

        size_t realLen = 32;
        while (realLen > 0 && plaintext[realLen - 1] == 0)
            realLen--;

        return std::string(reinterpret_cast<char*>(plaintext), realLen);
    }


    Encryptor::Encryptor(EncryptionAlgorithm* algorithm, unsigned char keyInput[32], const unsigned char ivInput[16])
        : encryptionAlgorithm(algorithm)
    {
        if (!algorithm) throw std::invalid_argument("Algorithm cannot be null");
        memcpy(this->key, keyInput, 32);
        memcpy(this->iv, ivInput, 16);
    }

    void Encryptor::setKey(const std::string& newKey) {
        memset(this->key, 0, 32);
        size_t len = std::min(newKey.size(), size_t(32));
        std::copy(newKey.begin(), newKey.begin() + len, this->key);
    }

    void Encryptor::setIv(const std::string& newIv) {
        memset(this->iv, 0, 16);
        size_t len = std::min(newIv.size(), size_t(16));
        std::copy(newIv.begin(), newIv.begin() + len, this->iv);
    }

    void Encryptor::setAlgorithm(EncryptionAlgorithm* algorithm) {
        if (!algorithm) throw std::invalid_argument("Algorithm cannot be null");
        this->encryptionAlgorithm = algorithm;
    }

    std::string Encryptor::encrypt(const std::string& data) {
        return encryptionAlgorithm->encrypt(data, key, iv);
    }

    std::string Encryptor::decrypt(const std::string& data) {
        return encryptionAlgorithm->decrypt(data, key, iv);
    }

}

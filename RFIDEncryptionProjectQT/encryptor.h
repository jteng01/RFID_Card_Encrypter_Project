#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <string>

namespace Crypto {

    class EncryptionAlgorithm {
    public:
        virtual ~EncryptionAlgorithm() = default;
        virtual std::string encrypt(const std::string& data, unsigned char key[], unsigned char iv[]) = 0;
        virtual std::string decrypt(const std::string& data, unsigned char key[], unsigned char iv[]) = 0;
    };

    class AESEncryption : public EncryptionAlgorithm {
    private:
        void handleErrors();

    public:
        std::string encrypt(const std::string& data, unsigned char key[], unsigned char iv[]) override;
        std::string decrypt(const std::string& data, unsigned char key[], unsigned char iv[]) override;
    };

    class Encryptor {
    private:
        EncryptionAlgorithm* encryptionAlgorithm;
        unsigned char key[32];
        unsigned char iv[16];

    public:
        Encryptor(EncryptionAlgorithm* algorithm, unsigned char keyInput[32], const unsigned char ivInput[16]);

        void setKey(const std::string& newKey);
        void setIv(const std::string& newIv);
        void setAlgorithm(EncryptionAlgorithm* algorithm);

        std::string encrypt(const std::string& data);
        std::string decrypt(const std::string& data);
    };

}

#endif

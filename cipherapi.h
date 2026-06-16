<<<<<<< HEAD
#pragma once
#include <vector>
#include <string>

class CipherAPI {
public:
    virtual ~CipherAPI() {}

    virtual std::vector<unsigned char> encrypt(
        const std::vector<unsigned char>& data,
        const std::string& key) = 0;

    virtual std::vector<unsigned char> decrypt(
        const std::vector<unsigned char>& data,
        const std::string& key) = 0;
};
=======
#ifndef CIPHERAPI_H
#define CIPHERAPI_H
#include <vector>
using ByteArray = std::vector<unsigned char>;

typedef ByteArray (*EncryptFunction)(const ByteArray&);
typedef ByteArray (*DecryptFunction)(const ByteArray&);

#endif
>>>>>>> 93828e04832088829754561e1cce934810836682

#pragma once

#include <vector>
#include <string>
#include "cipherapi.h"

using namespace std;

namespace SenCipher{
    int gcd(int a, int b);
    int modInverse(int a, int mod);

    vector<unsigned char> encryptAffine(
        const vector<unsigned char>& data,
        int a,
        int b);

    vector<unsigned char> decryptAffine(
        const vector<unsigned char>& data,
        int a,
        int b);
}

class AffineCipher : public CipherAPI {
public:
    vector<unsigned char> encrypt(
        const vector<unsigned char>& data,
        const string& key) override;

    vector<unsigned char> decrypt(
        const vector<unsigned char>& data,
        const string& key) override;
};

extern "C" __declspec(dllexport) CipherAPI* createCipher();
extern "C" __declspec(dllexport) void releaseCipher(CipherAPI* cipher);
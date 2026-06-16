#ifndef HILLCIPHER_H
#define HILLCIPHER_H

#include <vector>
#include <string>
#include "cipherapi.h"

using namespace std;

class HillCipher : public CipherAPI {
public:
    vector<unsigned char> encrypt(
        const vector<unsigned char>& data,
        const string& key) override;

    vector<unsigned char> decrypt(
        const vector<unsigned char>& data,
        const string& key) override;
};

namespace SenCipher{
    vector<unsigned char> encryptHill(
        const vector<unsigned char>& data,
        const vector<vector<int>>& key);

    vector<unsigned char> decryptHill(
        const vector<unsigned char>& data,
        const vector<vector<int>>& key);

    vector<vector<int>> inverseMatrix2x2(
        const vector<vector<int>>& matrix);
}

extern "C" __declspec(dllexport)
CipherAPI* createCipher();

extern "C" __declspec(dllexport)
void releaseCipher(CipherAPI* cipher);

#endif
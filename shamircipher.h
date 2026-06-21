#pragma once
#include <vector>
#include "cipherapi.h"
#include <string>
using namespace std;

class ShamirCipher : public CipherAPI {
public:
    vector<unsigned char> encrypt(
        const vector<unsigned char>& data,
        const string& key) override;

    vector<unsigned char> decrypt(
        const vector<unsigned char>& data,
        const string& key) override;
};
extern "C" __declspec(dllexport)
CipherAPI* createCipher();
namespace SenCipher{
vector<unsigned char> encryptShamir(
    const vector<unsigned char>& data,
    int p,
    int ca,
    int da,
    int cb);

vector<unsigned char> decryptShamir(
    const vector<unsigned char>& data,
    int p,
    int db);
}


extern "C" __declspec(dllexport)
void releaseCipher(CipherAPI* cipher);
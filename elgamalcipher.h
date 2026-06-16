#pragma once
#include <vector>
#include <string>
#include "cipherapi.h"
using namespace std;

class ElGamalCipher : public CipherAPI {
public:
    vector<unsigned char> encrypt(
        const vector<unsigned char>& data,
        const string& key) override;

    vector<unsigned char> decrypt(
        const vector<unsigned char>& data,
        const string& key) override;
};
namespace SenCipher {
vector<unsigned char> encryptElGamal(
    const vector<unsigned char>& data,
    int p,
    int g,
    int y);

vector<unsigned char> decryptElGamal(
    const vector<unsigned char>& data,
    int p,
    int x);
}
extern "C" __declspec(dllexport)
CipherAPI* createCipher();

extern "C" __declspec(dllexport)
void releaseCipher(CipherAPI* cipher);
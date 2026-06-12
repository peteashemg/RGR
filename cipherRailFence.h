#pragma once
#include "cipherAPI.h"

using namespace std;

//класс шифра Зигзаг, наследуемый от интерфейса
class CipherRailFence : public CipherAPI {
public:
    vector<unsigned char> encrypt(const vector<unsigned char>& data, const string& key) override;
    vector<unsigned char> decrypt(const vector<unsigned char>& data, const string& key) override;
};

//экспортные функции для сборки в DLL
extern "C" __declspec(dllexport) CipherAPI* createCipher();
extern "C" __declspec(dllexport) void releaseCipher(CipherAPI* cipher);

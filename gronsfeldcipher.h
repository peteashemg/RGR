#pragma once
#include "cipherapi.h"

using namespace std;

//класс шифра Гронсфельда, наследуемый от нашего  интерфейса
class CipherGronsfeld : public CipherAPI {
public:
    vector<unsigned char> encrypt(const vector<unsigned char>& data, const string& key) override;
    vector<unsigned char> decrypt(const vector<unsigned char>& data, const string& key) override;
};
/*
//функции, которые мы будем экспортировать в DLL для GetProcAddress
extern "C" CipherAPI* createCipher();
extern "C" void releaseCipher(CipherAPI* cipher);
*/
#pragma once
#include <string>
#include <vector>

using namespace std;

class CipherAPI {
public:
    virtual ~CipherAPI() {}
    virtual vector<unsigned char> encrypt(const vector<unsigned char>& data, const string& key) = 0;
    virtual vector<unsigned char> decrypt(const vector<unsigned char>& data, const string& key) = 0;
};

typedef CipherAPI* (*CreateCipherFunc)();
typedef void (*ReleaseCipherFunc)(CipherAPI*);

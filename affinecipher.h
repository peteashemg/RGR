#ifndef SENCIPHER_H
#define SENCIPHER_H
#include <vector>
using namespace std;

namespace SenCipher
{
    int gcd(int a, int b);
    int modInverse(int a, int mod);
    vector<unsigned char> encryptAffine(const vector<unsigned char>& data, int a, int b);
    vector<unsigned char> decryptAffine(const vector<unsigned char>& data, int a, int b);
}

#endif
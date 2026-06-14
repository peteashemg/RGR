#ifndef PUBLICKEYCIPHER_H
#define PUBLICKEYCIPHER_H

#include <vector>

using namespace std;

namespace SenCipher{
    struct ShamirUserKey{
        int c;
        int d;
    };

    struct ShamirKeys{
        int p;
        ShamirUserKey alice;
        ShamirUserKey bob;
    };

    struct ElGamalKey{
        int p;
        int g;
        int x;
        int y;
    };

    bool isPrime(int value);
    long long modPow(long long base, long long power, long long mod);
    ShamirKeys generateShamirKeys(int p);
    ElGamalKey generateElGamalKey(int p, int g);
    vector<unsigned char> encryptShamir(const vector<unsigned char>& data, int p, int ca, int da, int cb);
    vector<unsigned char> decryptShamir(const vector<unsigned char>& data, int p, int db);
    vector<unsigned char> encryptElGamal(const vector<unsigned char>& data, int p, int g, int y);
    vector<unsigned char> decryptElGamal(const vector<unsigned char>& data, int p, int x);
}

#endif

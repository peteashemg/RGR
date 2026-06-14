#include "affinecipher.h"
using namespace std;

namespace SenCipher{
    int gcd(int a, int b){
        while (b != 0){
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    int modInverse(int a, int mod){
        a %= mod;
        for (int x = 1; x < mod; x++){
            if ((a * x) % mod == 1){
                return x;
            }
        }
        return -1;
    }
    vector<unsigned char> encryptAffine(const vector<unsigned char>& data, int a, int b){
        vector<unsigned char> result;
        result.reserve(data.size());
        for (unsigned char byte : data){
            int encrypted = (a * static_cast<int>(byte) + b) % 256;
            result.push_back(static_cast<unsigned char>(encrypted)
            );
        }
        return result;
    }
    vector<unsigned char> decryptAffine(const vector<unsigned char>& data, int a, int b){
        vector<unsigned char> result;
        result.reserve(data.size());
        int inverseA = modInverse(a, 256);
        if (inverseA == -1){
            return {};
        }
        for (unsigned char byte : data){
            int value =
                inverseA *
                (static_cast<int>(byte) - b);
            value %= 256;
            if (value < 0){
                value += 256;
            }
            result.push_back(
                static_cast<unsigned char>(value)
            );
        }
        return result;
    }
}
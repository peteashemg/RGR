#include "elgamalcipher.h"
#include "affinecipher.h"
#include <sstream>
#include <random>
#include <stdexcept>
#include "mathutils.h"

using namespace std;

namespace SenCipher{

namespace{

const int INT_BYTES = 4;

bool isPrime(int value){
    if(value < 2){
        return false;
    }

    if(value == 2){
        return true;
    }

    if(value % 2 == 0){
        return false;
    }

    for(int i = 3; i * i <= value; i += 2){
        if(value % i == 0){
            return false;
        }
    }

    return true;
}

void checkPrimeModule(int p){
    if(!isPrime(p) || p <= 255){
        throw runtime_error(
            "p должно быть простым числом больше 255."
        );
    }
}

void appendInt(
    vector<unsigned char>& data,
    int value){

    for(int i = 0; i < INT_BYTES; i++){

        data.push_back(
            static_cast<unsigned char>(
                (value >> (8 * i)) & 255
            )
        );
    }
}

int readInt(
    const vector<unsigned char>& data,
    size_t position){

    int value = 0;

    for(int i = 0; i < INT_BYTES; i++){

        value |=
            static_cast<int>(
                data[position + i]
            ) << (8 * i);
    }

    return value;
}

}

vector<unsigned char> encryptElGamal(
    const vector<unsigned char>& data,
    int p,
    int g,
    int y){

    checkPrimeModule(p);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int>
        dist(2,p - 2);

    vector<unsigned char> result;

    for(unsigned char byte : data){

        int k;

        do{
            k = dist(gen);
        }
        while(gcd(k,p - 1) != 1);

        int a = modPow(g,k,p);

        int b =
            (modPow(y,k,p) *
             static_cast<int>(byte))
            % p;

        appendInt(result,a);
        appendInt(result,b);
    }

    return result;
}

vector<unsigned char> decryptElGamal(
    const vector<unsigned char>& data,
    int p,
    int x){

    checkPrimeModule(p);

    vector<unsigned char> result;

    for(size_t i = 0;
        i < data.size();
        i += INT_BYTES * 2){

        int a = readInt(data,i);

        int b =
            readInt(
                data,
                i + INT_BYTES
            );

        int m =
            (b *
            modPow(
                a,
                p - 1 - x,
                p
            )) % p;

        result.push_back(
            static_cast<unsigned char>(m)
        );
    }

    return result;
}

}
vector<unsigned char> ElGamalCipher::encrypt(
    const vector<unsigned char>& data,
    const string& key)
{
    stringstream ss(key);

    int p;
    int g;
    int y;

    ss >> p >> g >> y;

    return SenCipher::encryptElGamal(
        data,
        p,
        g,
        y
    );
}

vector<unsigned char> ElGamalCipher::decrypt(
    const vector<unsigned char>& data,
    const string& key)
{
    stringstream ss(key);

    int p;
    int x;

    ss >> p >> x;

    return SenCipher::decryptElGamal(
        data,
        p,
        x
    );
}

CipherAPI* createCipher()
{
    return new ElGamalCipher();
}

void releaseCipher(CipherAPI* cipher)
{
    delete cipher;
}
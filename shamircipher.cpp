#include "shamircipher.h"
#include "affinecipher.h"
#include <stdexcept>
#include <sstream>
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
        throw runtime_error("p должно быть простым числом больше 255.");
    }
}

void appendInt(vector<unsigned char>& data,int value){
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
            static_cast<int>(data[position + i])
            << (8 * i);
    }

    return value;
}

}

vector<unsigned char> encryptShamir(
    const vector<unsigned char>& data,
    int p,
    int ca,
    int da,
    int cb){

    checkPrimeModule(p);

    if(gcd(ca,p - 1) != 1 ||
       (ca * da) % (p - 1) != 1){

        throw runtime_error(
            "Ключи Алисы неверны."
        );
    }

    if(gcd(cb,p - 1) != 1){
        throw runtime_error(
            "Ключ Боба неверен."
        );
    }

    vector<unsigned char> result;

    for(unsigned char byte : data){

        int m = byte;

        int x1 = modPow(m,ca,p);
        int x2 = modPow(x1,cb,p);
        int x3 = modPow(x2,da,p);

        appendInt(result,x3);
    }

    return result;
}

vector<unsigned char> decryptShamir(
    const vector<unsigned char>& data,
    int p,
    int db){

    checkPrimeModule(p);

    vector<unsigned char> result;

    for(size_t i = 0;
        i < data.size();
        i += INT_BYTES){

        int x3 = readInt(data,i);

        int m = modPow(x3,db,p);

        result.push_back(
            static_cast<unsigned char>(m)
        );
    }

    return result;
}

}

vector<unsigned char> ShamirCipher::encrypt(
    const vector<unsigned char>& data,
    const string& key)
{
    stringstream ss(key);

    int p;
    int ca;
    int da;
    int cb;

    ss >> p >> ca >> da >> cb;

    return SenCipher::encryptShamir(
        data,
        p,
        ca,
        da,
        cb
    );
}

vector<unsigned char> ShamirCipher::decrypt(
    const vector<unsigned char>& data,
    const string& key){
    stringstream ss(key);
    int p;
    int db;
    ss >> p >> db;
    return SenCipher::decryptShamir(
        data,
        p,
        db
    );
}

CipherAPI* createCipher(){
    return new ShamirCipher();
}

void releaseCipher(CipherAPI* cipher){
    delete cipher;
}
#include "keygenerator.h"
#include "affinecipher.h"
#include <random>
#include <stdexcept>
using namespace std;

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

int modPow(int base,int power,int mod){

    int result = 1;

    base %= mod;

    while(power > 0){

        if(power % 2 == 1){
            result = (result * base) % mod;
        }

        base = (base * base) % mod;

        power /= 2;
    }

    return result;
}
namespace SenCipher{
    AffineKey generateAffineKey(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> distA(1, 255);
        uniform_int_distribution<int> distB(0, 255);
        AffineKey key;
        do{key.a = distA(gen);
        }
        while (gcd(key.a, 256) != 1);
        key.b = distB(gen);
        return key;
    }

    vector<vector<int>> generateHillKey(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(0, 255);
        vector<vector<int>> key(2,vector<int>(2));
        while (true){
            for (auto& row : key){
                for (auto& value : row){
                    value = dist(gen);
                }
            }
            int determinant =
                key[0][0] * key[1][1] -
                key[0][1] * key[1][0];

            determinant %= 256;
            if (determinant < 0){
                determinant += 256;
            }
            if (gcd(determinant, 256) == 1){
                return key;
            }
        }
    }
    ShamirKeys generateShamirKeys(int p){
        checkPrimeModule(p);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(2, p - 2);

        ShamirKeys keys;
        keys.p = p;

        // Для каждого участника выбираем c, взаимно простое с p - 1, и находим d.
        do{
            keys.alice.c = dist(gen);
        }
        while (gcd(keys.alice.c, p - 1) != 1);
        keys.alice.d = modInverse(keys.alice.c, p - 1);

        do{
            keys.bob.c = dist(gen);
        }
        while (gcd(keys.bob.c, p - 1) != 1);
        keys.bob.d = modInverse(keys.bob.c, p - 1);

        return keys;
    }

    ElGamalKey generateElGamalKey(int p, int g){
        checkPrimeModule(p);
        if (g <= 1 || g >= p){
            throw runtime_error("g должно быть больше 1 и меньше p.");
        }

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(2, p - 2);

        ElGamalKey key;
        key.p = p;
        key.g = g;
        key.x = dist(gen);
        key.y = static_cast<int>(modPow(g, key.x, p));
        return key;
    }
    
    string generateGronsfeldKey(int length){
        random_device rd;
        mt19937 gen(rd());

        uniform_int_distribution<int> dist(0,9);

        string key;

        for(int i = 0; i < length; i++){
            key += char('0' + dist(gen));
        }
        return key;
    }
    int generateRailFenceKey(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(2,10);
        return dist(gen);
    }
}
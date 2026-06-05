#include "keygenerator.h"
#include "sencipher.h"
#include <random>
using namespace std;

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
}
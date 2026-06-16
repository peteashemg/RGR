#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H
#include <vector>
<<<<<<< HEAD
#include <string>
=======
>>>>>>> 93828e04832088829754561e1cce934810836682
using namespace std;

namespace SenCipher{
    struct AffineKey{
        int a; int b;
    };
    AffineKey generateAffineKey();
    vector<vector<int>> generateHillKey();
<<<<<<< HEAD
    struct ShamirUserKey{
        int c;
        int d;
    };
    struct ShamirKeys{
        int p;
        ShamirUserKey alice;
        ShamirUserKey bob;
    };
    ShamirKeys generateShamirKeys(int p);
    struct ElGamalKey
        {
    int p;
    int g;
    int x;
    int y;
    };
    ElGamalKey generateElGamalKey(int p, int g);
    string generateGronsfeldKey(int length);
    int generateRailFenceKey();
=======
>>>>>>> 93828e04832088829754561e1cce934810836682
}
#endif
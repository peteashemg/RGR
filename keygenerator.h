#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H
#include <vector>
using namespace std;

namespace SenCipher{
    struct AffineKey{
        int a; int b;
    };
    AffineKey generateAffineKey();
    vector<vector<int>> generateHillKey();
}
#endif
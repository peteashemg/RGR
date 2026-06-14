#pragma once
#include <vector>
using namespace std;

namespace SenCipher {
vector<unsigned char> encryptElGamal(
    const vector<unsigned char>& data,
    int p,
    int g,
    int y);

vector<unsigned char> decryptElGamal(
    const vector<unsigned char>& data,
    int p,
    int x);
}
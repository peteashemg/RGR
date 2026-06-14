#pragma once
#include <vector>
using namespace std;

namespace SenCipher{
vector<unsigned char> encryptShamir(
    const vector<unsigned char>& data,
    int p,
    int ca,
    int da,
    int cb);

vector<unsigned char> decryptShamir(
    const vector<unsigned char>& data,
    int p,
    int db);
}
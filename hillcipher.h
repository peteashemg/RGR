#ifndef HILLCIPHER_H
#define HILLCIPHER_H
#include <vector>
using namespace std;

namespace SenCipher{
    vector<unsigned char> encryptHill(const vector<unsigned char>& data, const vector<vector<int>>& key);
    vector<unsigned char> decryptHill(const vector<unsigned char>& data, const vector<vector<int>>& key);
    vector<vector<int>> inverseMatrix2x2(const vector<vector<int>>& matrix);
}
#endif
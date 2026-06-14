#include "hillcipher.h"
#include "affinecipher.h"
using namespace std;

namespace SenCipher{
    vector<vector<int>> inverseMatrix2x2(const vector<vector<int>>& matrix){
        int det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        det %= 256;
        if (det < 0){
            det += 256;
        }
        int detInverse = modInverse(det, 256);
        if (detInverse == -1){
            return {};
        }
        vector<vector<int>> result(2, vector<int>(2)
        );
        result[0][0] = matrix[1][1];
        result[0][1] = -matrix[0][1];
        result[1][0] = -matrix[1][0];
        result[1][1] = matrix[0][0];
        for (auto& row : result){
            for (auto& value : row){
                value *= detInverse;
                value %= 256;

                if (value < 0){
                    value += 256;
                }
            }
        }
        return result;
    }

    vector<unsigned char> encryptHill(const vector<unsigned char>& data, const vector<vector<int>>& key){
        vector<unsigned char> result;
        vector<unsigned char> prepared = data;
        if (prepared.size() % 2 != 0){
            prepared.push_back(0);
        }
        result.reserve(prepared.size());
        for (size_t i = 0; i < prepared.size(); i += 2){
            int b1 = static_cast<int>(prepared[i]);
            int b2 = static_cast<int>(prepared[i + 1]);
            int c1 = (key[0][0] * b1 + key[0][1] * b2) % 256;
            int c2 = (key[1][0] * b1 + key[1][1] * b2) % 256;
            result.push_back(
                static_cast<unsigned char>(c1)
            );
            result.push_back(
                static_cast<unsigned char>(c2)
            );
        }
        return result;
    }

    vector<unsigned char> decryptHill(const vector<unsigned char>& data, const vector<vector<int>>& key
    )
    {
        vector<vector<int>> inverseKey = inverseMatrix2x2(key);
        if (inverseKey.empty()){
            return {};
        }

        vector<unsigned char> result;
        result.reserve(data.size());
        for (size_t i = 0; i < data.size(); i += 2){
            int b1 = static_cast<int>(data[i]);
            int b2 = static_cast<int>(data[i + 1]);
            int p1 = (inverseKey[0][0] * b1 + inverseKey[0][1] * b2 ) % 256;
            int p2 = (inverseKey[1][0] * b1 + inverseKey[1][1] * b2) % 256;
            if (p1 < 0){
                p1 += 256;
            }
            if (p2 < 0){
                p2 += 256;
            }
            result.push_back(static_cast<unsigned char>(p1));
            result.push_back(static_cast<unsigned char>(p2));
        }
        return result;
    }
}
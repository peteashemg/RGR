#include "publickeycipher.h"
#include "sencipher.h"
#include <random>
#include <stdexcept>

using namespace std;

namespace SenCipher{
    namespace{
        const int INT_BYTES = 4;

        void checkPrimeModule(int p){
            if (!isPrime(p) || p <= 255){
                throw runtime_error("p должно быть простым числом больше 255.");
            }
        }

        void appendInt(vector<unsigned char>& data, long long value){
            // Сохраняем каждое число в 4 байта, чтобы результат можно было записать в файл.
            for (int i = 0; i < INT_BYTES; i++){
                data.push_back(static_cast<unsigned char>((value >> (8 * i)) & 255));
            }
        }

        long long readInt(const vector<unsigned char>& data, size_t position){
            long long value = 0;
            for (int i = 0; i < INT_BYTES; i++){
                value |= static_cast<long long>(data[position + i]) << (8 * i);
            }
            return value;
        }
    }

    bool isPrime(int value){
        if (value < 2){
            return false;
        }
        if (value == 2){
            return true;
        }
        if (value % 2 == 0){
            return false;
        }
        for (int divisor = 3; divisor * divisor <= value; divisor += 2){
            if (value % divisor == 0){
                return false;
            }
        }
        return true;
    }

    long long modPow(long long base, long long power, long long mod){
        long long result = 1;
        base %= mod;
        while (power > 0){
            if (power % 2 == 1){
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            power /= 2;
        }
        return result;
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

    vector<unsigned char> encryptShamir(const vector<unsigned char>& data, int p, int ca, int da, int cb){
        checkPrimeModule(p);
        if (gcd(ca, p - 1) != 1 || (ca * da) % (p - 1) != 1){
            throw runtime_error("Ключи Алисы cA и dA должны быть взаимно обратными по mod (p - 1).");
        }
        if (gcd(cb, p - 1) != 1){
            throw runtime_error("Ключ Боба cB должен быть взаимно простым с p - 1.");
        }

        vector<unsigned char> result;
        result.reserve(data.size() * INT_BYTES);
        for (unsigned char byte : data){
            long long m = static_cast<int>(byte);

            // Протокол Шамира: A шифрует, B шифрует, A снимает свой слой.
            long long x1 = modPow(m, ca, p);
            long long x2 = modPow(x1, cb, p);
            long long x3 = modPow(x2, da, p);
            appendInt(result, x3);
        }
        return result;
    }

    vector<unsigned char> decryptShamir(const vector<unsigned char>& data, int p, int db){
        checkPrimeModule(p);
        if (data.size() % INT_BYTES != 0){
            throw runtime_error("Данные Шамира повреждены: размер не кратен 4 байтам.");
        }

        vector<unsigned char> result;
        result.reserve(data.size() / INT_BYTES);
        for (size_t i = 0; i < data.size(); i += INT_BYTES){
            long long x3 = readInt(data, i);
            long long m = modPow(x3, db, p);
            if (m < 0 || m > 255){
                throw runtime_error("После расшифрования получено значение вне диапазона байта.");
            }
            result.push_back(static_cast<unsigned char>(m));
        }
        return result;
    }

    vector<unsigned char> encryptElGamal(const vector<unsigned char>& data, int p, int g, int y){
        checkPrimeModule(p);
        if (g <= 1 || g >= p || y <= 0 || y >= p){
            throw runtime_error("Параметры g и y должны находиться в диапазоне от 1 до p.");
        }

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(2, p - 2);

        vector<unsigned char> result;
        result.reserve(data.size() * INT_BYTES * 2);
        for (unsigned char byte : data){
            int k;
            do{
                k = dist(gen);
            }
            while (gcd(k, p - 1) != 1);

            // Для каждого байта создается своя пара (a, b).
            long long a = modPow(g, k, p);
            long long b = (modPow(y, k, p) * static_cast<int>(byte)) % p;
            appendInt(result, a);
            appendInt(result, b);
        }
        return result;
    }

    vector<unsigned char> decryptElGamal(const vector<unsigned char>& data, int p, int x){
        checkPrimeModule(p);
        if (x <= 0 || x >= p - 1){
            throw runtime_error("Закрытый ключ x должен быть в диапазоне от 1 до p - 2.");
        }
        if (data.size() % (INT_BYTES * 2) != 0){
            throw runtime_error("Данные Эль-Гамаля повреждены: размер не кратен 8 байтам.");
        }

        vector<unsigned char> result;
        result.reserve(data.size() / (INT_BYTES * 2));
        for (size_t i = 0; i < data.size(); i += INT_BYTES * 2){
            long long a = readInt(data, i);
            long long b = readInt(data, i + INT_BYTES);

            // a^(p - 1 - x) является обратным элементом к a^x по малой теореме Ферма.
            long long m = (b * modPow(a, p - 1 - x, p)) % p;
            if (m < 0 || m > 255){
                throw runtime_error("После расшифрования получено значение вне диапазона байта.");
            }
            result.push_back(static_cast<unsigned char>(m));
        }
        return result;
    }
}

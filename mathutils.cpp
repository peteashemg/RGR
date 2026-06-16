#include "mathutils.h"

namespace SenCipher
{
    int gcd(int a, int b)
    {
        while (b != 0)
        {
            int t = b;
            b = a % b;
            a = t;
        }
        return a;
    }

    int modInverse(int a, int mod)
    {
        for (int x = 1; x < mod; x++)
        {
            if ((a * x) % mod == 1)
                return x;
        }
        return -1;
    }

    int modPow(int base, int exp, int mod)
    {
        int result = 1;

        while (exp > 0)
        {
            if (exp & 1)
                result = (result * base) % mod;

            base = (base * base) % mod;
            exp >>= 1;
        }

        return result;
    }
}
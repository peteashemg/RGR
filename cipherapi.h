#ifndef CIPHERAPI_H
#define CIPHERAPI_H
#include <vector>
using ByteArray = std::vector<unsigned char>;

typedef ByteArray (*EncryptFunction)(const ByteArray&);
typedef ByteArray (*DecryptFunction)(const ByteArray&);

#endif
#include "gronsfeldcipher.h"
#include <stdexcept>

using namespace std;

//алгоритм шифрования Гронсфельда (побайтное смещение на цифры ключа )
vector<unsigned char> CipherGronsfeld::encrypt(const vector<unsigned char>& data, const string& key) {
    if (key.empty()) throw invalid_argument("Ключ не может быть пустым!");
    
    vector<unsigned char> result;
    result.reserve(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        // берем цифру из ключа (символ переводим в число от 0 до 9)
        int shift = key[i % key.size()] - '0';
        if (shift < 0 || shift > 9) {
            throw invalid_argument("Ключ шифра Гронсфельда должен состоять только из цифр");
        }
        // сдвигаем байт с учетом переполнения (размер таблицы =256)
        result.push_back((data[i] + shift) % 256);
    }
    return result;
}

//флгоритм расшифрования Гронсфельда (обратное побайтное смещение)
vector<unsigned char> CipherGronsfeld::decrypt(const vector<unsigned char>& data, const string& key) {
    if (key.empty()) throw invalid_argument("Ключ не может быть пустым!");
    
    vector<unsigned char> result;
    result.reserve(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        int shift = key[i % key.size()] - '0';
        if (shift < 0 || shift > 9) {
            throw invalid_argument("Ключ шифра Гронсфельда должен состоять только из цифр!");
        }
        //вычитаем сдвиг, добавляя 256 , чтобы избежать отрицательных чисел при делении
        result.push_back((data[i] - shift + 256) % 256);
    }
    return result;
}

//функции создания и удаления объектов для DLL библиотеки
CipherAPI* createCipher() {
    return new CipherGronsfeld();
}

void releaseCipher(CipherAPI* cipher) {
    delete cipher;
}

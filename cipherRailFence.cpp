#include "cipherRailFence.h"
#include <stdexcept>

using namespace std;

//шифрование зигзагом (Rail fence)
vector<unsigned char> CipherRailFence::encrypt(const vector<unsigned char>& data, const string& key) {
    if (data.empty()) return data;
    
    int numRails = 0;
    try {
        numRails = stoi(key); //переводим строку в число
    } catch (...) {
        throw invalid_argument("Ключ для шифра Зигзаг должен быть целым числом");
    }
    
    if (numRails <= 1) throw invalid_argument("Количество строк должно быть больше 1");

//массив векторов для построения зигзага
    vector<vector<unsigned char>> fence(numRails);
    int rail = 0;
    bool directionDown = false;

// распределяем байты по волне зигзага
    for (size_t i = 0; i < data.size(); ++i) {
        fence[rail].push_back(data[i]);
        
        if (rail == 0 || rail == numRails - 1) {
            directionDown = !directionDown; // Меняем направление движения
        }
        rail += directionDown ? 1 : -1;
    }

//собираем строки в один итоговый вектор
    vector<unsigned char> result;
    result.reserve(data.size());
    for (int i = 0; i < numRails; ++i) {
        for (size_t j = 0; j < fence[i].size(); ++j) {
            result.push_back(fence[i][j]);
        }
    }
    return result;
}

//дешифрование методом зигзаг 
vector<unsigned char> CipherRailFence::decrypt(const vector<unsigned char>& data, const string& key) {
    if (data.empty()) return data;

    int numRails = 0;
    try {
        numRails = stoi(key);
    } catch (...) {
        throw invalid_argument("Ключ для шифра Зигзаг должен быть целым числом!");
    }

    if (numRails <= 1) throw invalid_argument("Количество строк должно быть больше 1!");

//матрица для разметки позиций символов
    vector<vector<bool>> marker(numRails, vector<bool>(data.size(), false));
    int rail = 0;
    bool directionDown = false;

    for (size_t i = 0; i < data.size(); ++i) {
        marker[rail][i] = true;
        if (rail == 0 || rail == numRails - 1) {
            directionDown = !directionDown;
        }
        rail += directionDown ? 1 : -1;
    }

// заполняем размеченный зигзаг элементами из зашифрованного вектора
    vector<vector<unsigned char>> fence(numRails);
    size_t dataIndex = 0;
    for (int i = 0; i < numRails; ++i) {
        for (size_t j = 0; j < data.size(); ++j) {
            if (marker[i][j] && dataIndex < data.size()) {
                fence[i].push_back(data[dataIndex++]);
            }
        }
    }

//собираем текст обратно , двигаясь по зигзагу
    vector<unsigned char> result;
    result.reserve(data.size());
    vector<size_t> rIndex(numRails, 0);
    rail = 0;
    directionDown = false;

    for (size_t i = 0; i < data.size(); ++i) {
        result.push_back(fence[rail][rIndex[rail]++]);
        if (rail == 0 || rail == numRails - 1) {
            directionDown = !directionDown;
        }
        rail += directionDown ? 1 : -1;
    }
    return result;
}

//функции создания и удаления объектов класса
CipherAPI* createCipher() {
    return new CipherRailFence();
}

void releaseCipher(CipherAPI* cipher) {
    delete cipher;
}

#include <iostream>
#include <string>
#include "sencipher.h"

using namespace std;

namespace Crypto {

    // Функция для поиска модульной инверсии (a^-1 mod m)
    // Используем только базовый int, как требуют условия лабы
    int findInverse(int a, int m) {
        a = a % m;
        for (int x = 1; x < m; x++) {
            if ((a * x) % m == 1) {
                return x; // Нашли мультипликативно обратное число
            }
        }
        return -1; // Если решения нет
    }

    // Функция аффинного шифрования
    string encryptAffine(string text, int a, int b) {
        string result = "";
        cout << "\n--- Процесс шифрования по шагам ---" << endl;

        for (int i = 0; i < text.length(); i++) {
            char c = text[i];

            // Проверка на заглавные английские буквы вручную (вместо isupper)
            if (c >= 'A' && c <= 'Z') {
                int p = c - 'A'; // Индекс от 0 до 25
                int cipherIdx = (a * p + b) % 26;
                char cipherChar = cipherIdx + 'A';
                
                cout << "Буква: " << c << " (индекс " << p << ") -> (" 
                     << a << " * " << p << " + " << b << ") mod 26 = " 
                     << cipherIdx << " -> " << cipherChar << endl;
                     
                result += cipherChar;
            }
            // Проверка на строчные английские буквы вручную (вместо islower)
            else if (c >= 'a' && c <= 'z') {
                int p = c - 'a'; // Индекс от 0 до 25
                int cipherIdx = (a * p + b) % 26;
                char cipherChar = cipherIdx + 'a';
                
                cout << "Буква: " << c << " (индекс " << p << ") -> (" 
                     << a << " * " << p << " + " << b << ") mod 26 = " 
                     << cipherIdx << " -> " << cipherChar << endl;
                     
                result += cipherChar;
            }
            // Все остальные символы (пробелы, знаки препинания) пропускаем без изменений
            else {
                result += c;
            }
        }
        return result;
    }

    // Функция аффинного дешифрования
    string decryptAffine(string text, int a, int b) {
        string result = "";
        int aInverse = findInverse(a, 26);

        if (aInverse == -1) {
            cout << "[Ошибка] Число 'a' не взаимно простое с 26! Дешифрование невозможно." << endl;
            return text;
        }

        cout << "\n--- Процесс дешифрования по шагам (a^-1 mod 26 = " << aInverse << ") ---" << endl;

        for (int i = 0; i < text.length(); i++) {
            char c = text[i];

            // Обработка заглавных букв вручную
            if (c >= 'A' && c <= 'Z') {
                int cipherIdx = c - 'A';
                // В C++ остаток от деления отрицательного числа может быть отрицательным,
                // поэтому аккуратно считаем формулу и нормализуем значение
                int p = (aInverse * (cipherIdx - b)) % 26;
                if (p < 0) {
                    p += 26;
                }
                char plainChar = p + 'A';
                
                cout << "Шифр-буква: " << c << " (индекс " << cipherIdx << ") -> " 
                     << aInverse << " * (" << cipherIdx << " - " << b << ") mod 26 = " 
                     << p << " -> " << plainChar << endl;
                     
                result += plainChar;
            }
            // Обработка строчных букв вручную
            else if (c >= 'a' && c <= 'z') {
                int cipherIdx = c - 'a';
                int p = (aInverse * (cipherIdx - b)) % 26;
                if (p < 0) {
                    p += 26;
                }
                char plainChar = p + 'a';
                
                cout << "Шифр-буква: " << c << " (индекс " << cipherIdx << ") -> " 
                     << aInverse << " * (" << cipherIdx << " - " << b << ") mod 26 = " 
                     << p << " -> " << plainChar << endl;
                     
                result += plainChar;
            }
            else {
                result += c;
            }
        }
        return result;
    }
}
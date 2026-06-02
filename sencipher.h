#ifndef SENCIPHER_H
#define SENCIPHER_H

#include <string>

// Все функции шифрования и утилиты инкапсулируем в понятное пространство имен
namespace SenCipher {
    
    // Поиск мультипликативно обратного числа (для дешифрования)
    int findInverse(int a, int m);
    
    // Функции шифрования и дешифрования строк (с выводом шагов)
    std::string encryptAffine(const std::string& text, int a, int b);
    std::string decryptAffine(const std::string& cipherText, int a, int b);
    
    // Функции для работы с файлами (требование методички)
    bool encryptFile(const std::string& inputPath, const std::string& outputPath, int a, int b);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath, int a, int b);
}

#endif
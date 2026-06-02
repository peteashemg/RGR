#include "affine_cipher.h"
#include <iostream>
#include <cctype>

// Классический алгоритм Евклида для проверки валидности ключа 'a'
int calculateGCD(int a, int b) {
    while (b != 0) {
        int remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

std::string encryptAffine(const std::string& text, int a, int b) {
    // Используем std::string, размер которого динамический (нет фиксированных массивов)
    std::string encryptedText = "";
    int m = 26; // Размер английского алфавита

    // Проверка ключа 'a' на взаимную простоту с 'm'
    if (calculateGCD(a, m) != 1) {
        std::cout << "[Ошибка] Ключ 'a' (" << a << ") и размер алфавита (" << m 
                  << ") не взаимно простые!" << std::endl;
        return "";
    }

    std::cout << "--- Промежуточные шаги шифрования (Аффинный шифр) ---" << std::endl;

    for (char c : text) {
        if (std::isalpha(c)) {
            // Приводим букву к верхнему регистру для удобства расчетов
            char upperChar = std::toupper(c);
            int x = upperChar - 'A'; // Получаем индекс от 0 до 25
            
            // Основная формула: E(x) = (a * x + b) mod m
            int encryptedX = (a * x + b) % m;
            
            // Если остаток отрицательный (в С++ такое бывает при отрицательных 'b'), корректируем его
            if (encryptedX < 0) {
                encryptedX += m;
            }

            char encryptedChar = encryptedX + 'A'; // Возвращаем в символ
            encryptedText += encryptedChar;

            // Вывод промежуточных вычислений с пояснениями
            std::cout << "Символ: '" << upperChar << "' (x = " << x << ") -> "
                      << "Формула: (" << a << " * " << x << " + " << b << ") mod " << m 
                      << " = " << encryptedX << " -> Зашифрованный символ: '" << encryptedChar << "'" << std::endl;
        } else {
            // Если это пробел или знак препинания, оставляем как есть
            encryptedText += c;
        }
    }
    
    std::cout << "-------------------------------------------------------" << std::endl;
    return encryptedText;
}
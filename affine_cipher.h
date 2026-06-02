#ifndef AFFINE_CIPHER_H
#define AFFINE_CIPHER_H

#include <string>

// Функция для нахождения НОД (наибольшего общего делителя)
// Используем стандартный тип int, избегая long long
int calculateGCD(int a, int b);

// Функция выполнения аффинного шифрования
// Принимает текст, ключи 'a' и 'b', возвращает зашифрованную строку
std::string encryptAffine(const std::string& text, int a, int b);

#endif // AFFINE_CIPHER_H
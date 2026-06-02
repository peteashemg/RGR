#include <iostream>
#include <fstream>
#include <string>
#include "affine_cipher.h"
// Позже сокомандники добавят сюда свои заголовочные файлы:
// #include "rsa_cipher.h"
// #include "shamir_cipher.h"

using namespace std;

int main() {
    int choice = 0;
    
    // Бесконечный цикл для удобного меню
    while (true) {
        cout << "\n=== Программа шифрования документов ===" << endl;
        cout << "Выберите метод шифрования:" << endl;
        cout << "1. Аффинный шифр (демонстрация)" << endl;
        cout << "2. Шифр RSA (в разработке)" << endl;
        cout << "3. Шифр Диффи-Хеллмана (в разработке)" << endl;
        cout << "4. Шифр Шамира (в разработке)" << endl;
        cout << "5. Шифр Эль-Гамаля (в разработке)" << endl;
        cout << "6. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        if (choice == 6) {
            cout << "Завершение работы программы." << endl;
            break;
        }

        string inputFileName, outputFileName;
        cout << "Введите имя файла для шифрования (с расширением, например input.txt): ";
        cin >> inputFileName;

        // Чтение исходного файла
        ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            cout << "[Ошибка] Не удалось открыть файл: " << inputFileName << ". Убедитесь, что он лежит в директории программы." << endl;
            continue; // Возврат в меню
        }

        // Считываем весь текст из файла динамически
        string fileContent = "";
        string line;
        while (getline(inputFile, line)) {
            fileContent += line + "\n";
        }
        inputFile.close();

        // Ветвление для вызова конкретных алгоритмов
        switch (choice) {
            case 1: {
                int keyA, keyB;
                cout << "Введите ключ 'a' (взаимно простое с 26): ";
                cin >> keyA;
                cout << "Введите ключ 'b' (сдвиг): ";
                cin >> keyB;

                string result = encryptAffine(fileContent, keyA, keyB);
                
                if (!result.empty()) {
                    outputFileName = "encrypted_" + inputFileName;
                    ofstream outputFile(outputFileName);
                    outputFile << result;
                    outputFile.close();
                    cout << "[Успех] Файл зашифрован и сохранен как: " << outputFileName << endl;
                }
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
                cout << "[Уведомление] Этот метод шифрования еще не добавлен вашими сокомандниками." << endl;
                break;
            default:
                cout << "[Ошибка] Неверный пункт меню." << endl;
        }
    }

    return 0;
}
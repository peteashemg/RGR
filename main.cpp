#include <iostream>
#include <fstream>
#include <string>
#include "sencipher.h"

// Требование: глобальное пространство имен вынесено в препроцессинг
using namespace std;
using namespace SenCipher;

int main() {
    // Настройка локализации для корректного отображения кириллицы в терминале
    setlocale(LC_ALL, "Russian");
    
    int choice;

    while (true) {
        cout << "\nПрограмма шифрования документов" << endl;
        cout << "Выберите метод шифрования:" << endl;
        cout << "1. Аффинный шифр" << endl;
        cout << "2. Шифр RSA (в разработке)" << endl;
        cout << "3. Шифр Диффи-Хеллмана (в разработке)" << endl;
        cout << "4. Шифр Шамира (в разработке)" << endl;
        cout << "5. Шифр Эль-Гамаля (в разработке)" << endl;
        cout << "6. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        if (choice == 6) {
            cout << "Завершение работы программы. До встречи!" << endl;
            break;
        }

        // Защита: проверка выбора перед тем, как запрашивать файл
        if (choice < 1 || choice > 5) {
            cout << "[Ошибка] Неверный пункт меню. Попробуйте снова." << endl;
            continue; 
        }

        string inputFileName, outputFileName;
        cout << "Введите имя файла для обработки (с расширением, например input.txt): ";
        cin >> inputFileName;

        // Чтение исходного файла
        ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            cout << "[Ошибка] Не удалось открыть файл: " << inputFileName << ". Убедитесь, что он лежит в директории RGR." << endl;
            continue; // Возврат в главное меню
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
                // --- АФФИННЫЙ ШИФР ---
                int mode;
                cout << "Выберите режим (1 - Шифрование, 2 - Дешифрование): ";
                cin >> mode;

                int keyA, keyB;
                cout << "Введите ключ 'a' (взаимно простое с 26): ";
                cin >> keyA;
                cout << "Введите ключ 'b' (сдвиг): ";
                cin >> keyB;

                string result = "";
                if (mode == 1) {
                    result = encryptAffine(fileContent, keyA, keyB);
                    outputFileName = "encrypted_" + inputFileName;
                } else if (mode == 2) {
                    result = decryptAffine(fileContent, keyA, keyB);
                    outputFileName = "decrypted_" + inputFileName;
                } else {
                    cout << "[Ошибка] Неверный режим работы." << endl;
                    break;
                }
                
                // Сохранение результата работы алгоритма в файл
                if (!result.empty()) {
                    ofstream outputFile(outputFileName);
                    if (outputFile.is_open()) {
                        outputFile << result;
                        outputFile.close();
                        cout << "[УСПЕХ] Файл успешно обработан и сохранен как: " << outputFileName << endl;
                    } else {
                        cout << "[Ошибка] Не удалось перезаписать итоговый файл." << endl;
                    }
                }
                break;
            }

            case 2: {

                cout << "Этот метод шифрования (RSA) еще не добавлен вашими сокомандниками." << endl;
                break;
            }

            case 3: {

                cout << "Этот метод шифрования (Диффи-Хеллман) еще не добавлен вашими сокомандниками." << endl;
                break;
            }

            case 4: {

                cout << "Этот метод шифрования (Шамир) еще не добавлен вашими сокомандниками." << endl;
                break;
            }

            case 5: {

                cout << "Этот метод шифрования (Эль-Гамаль) еще не добавлен вашими сокомандниками." << endl;
                break;
            }

            default:
                cout << "[Ошибка] Непредвиденный сбой логики меню." << endl;
        }
    }

    return 0;
}
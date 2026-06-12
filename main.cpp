#include <iostream>
#include <string>
#include <windows.h>
#include "cipherAPI.h"

using namespace std;

void processCipher(const string& dllName) {
    HMODULE hDll = LoadLibraryA(dllName.c_str());
    if (!hDll) {
        cout << "[Ошибка] Не удалось загрузить библиотеку: " << dllName << endl;
        return;
    }

    CreateCipherFunc createCipher = (CreateCipherFunc)GetProcAddress(hDll, "createCipher");
    ReleaseCipherFunc releaseCipher = (ReleaseCipherFunc)GetProcAddress(hDll, "releaseCipher");

    if (!createCipher || !releaseCipher) {
        cout << "[Ошибка] Неверная структура DLL библиотеки!" << endl;
        FreeLibrary(hDll);
        return;
    }

    CipherAPI* cipher = createCipher();
    
    cin.ignore();
    cout << "Введите текст для шифрования: ";
    string text;
    getline(cin, text);

    cout << "Введите ключ: ";
    string key;
    getline(cin, key);

    vector<unsigned char> data(text.begin(), text.end()); //приведение к вектору байт

    vector<unsigned char> encrypted = cipher->encrypt(data, key); // вызов функций шифра
    string encStr(encrypted.begin(), encrypted.end());
    cout << "Зашифрованный текст: " << encStr << endl;

    vector<unsigned char> decrypted = cipher->decrypt(encrypted, key);
    string decStr(decrypted.begin(), decrypted.end());
    cout << "Расшифрованный текст: " << decStr << endl;

    releaseCipher(cipher);
    FreeLibrary(hDll);
}


void showMenu() {
    cout << "\n Расчетно-графическая работа" << endl;
    cout << "1. Шифр Гронсфельда (Gronsfeld Cipher) [DLL]" << endl;
    cout << "2. Шифр Зигзаг (Rail Fence) [DLL]" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите пункт: ";
}

int main() {
    cout << "Система шифрования данных" << endl;
    
    int choice = -1;
    while (choice != 0) {
        showMenu();
        if (!(cin >> choice)) {
            cout << "Ошибка. Введите число" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                processCipher("gronsfeld.dll");
                break;
            case 2:
                processCipher("railfence.dll");
                break;
            case 0:
                cout << "Завершение работы программы" << endl;
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова" << endl;
        }
    }
    return 0;
}

#include <iostream>
#include <string>
#include <windows.h>
#include "cipherAPI.h"

using namespace std;

void processCipher(const string& dllName) { // движок загрузки библиотек 
    HMODULE hDll = LoadLibraryA(dllName.c_str());
    if (!hDll) {
        cout << "[Ошибка] Не удалось загрузить библиотеку: " << dllName << endl;
        return;
    }

    CreateCipherFunc createCipher = (CreateCipherFunc)GetProcAddress(hDll, "createCipher");
    ReleaseCipherFunc releaseCipher = (ReleaseCipherFunc)GetProcAddress(hDll, "releaseCipher");

    if (!createCipher || !releaseCipher) {
        cout << "Ошибка. Неверная структура DLL библиотеки" << endl;
        FreeLibrary(hDll);
        return;
    }

    cout << "Успех. Библиотека " << dllName << " успешно найдена и загружена" << endl;
    
    FreeLibrary(hDll); // Освобождаем ресурсы
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

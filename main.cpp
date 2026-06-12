#include <iostream>
#include <string>
#include "cipherAPI.h"

using namespace std;

void showMenu() {
    cout << "\nРасчетно-графическая работа" << endl;
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
                cout << "\n[Инфо] Выбран шифр Гронсфельда. " << endl; //Логика будет добавлена позже.
                break;
            case 2:
                cout << "\n[Инфо] Выбран шифр Rail Fence. " << endl; // Логика будет добавлена позжеф.
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

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cstdio>
#include "sencipher.h"
#include "hillcipher.h"
#include "keygenerator.h"
using namespace std;
using namespace SenCipher;

void waitForEnter(){
    cout << "\n====================================\n";
    cout << "Нажмите Enter для продолжения...";
    cout << "\n====================================\n";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.get();
}
void printSuccess(const string& message){
    cout << "\n====================================\n";
    cout << "[УСПЕХ]\n";
    cout << message << endl;
    cout << "====================================\n";
}
void printError(const string& message){
    cout << "\n====================================\n";
    cout << "[ОШИБКА]\n";
    cout << message << endl;
    cout << "====================================\n";
}
vector<unsigned char> readFile(const string& fileName){
    ifstream file(fileName, ios::binary);
    if (!file){
        throw runtime_error("Не удалось открыть файл.");
    }
    return vector<unsigned char>(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}
void writeFile(const string& fileName, const vector<unsigned char>& data){
    ofstream file(fileName, ios::binary);
    if (!file){
        throw runtime_error("Не удалось создать файл.");
    }
    file.write(reinterpret_cast<const char*>(data.data()),static_cast<streamsize>(data.size()));
}
vector<unsigned char> stringToBytes(const string& text){
    return vector<unsigned char>(text.begin(), text.end());
}
vector<unsigned char> hexToBytes(const string& hexString){
    vector<unsigned char> result;
    for (size_t i = 0; i < hexString.length(); i += 2){
        string part = hexString.substr(i, 2);
        result.push_back(static_cast<unsigned char>(stoi(part, nullptr, 16)));
    }

    return result;
}
string bytesToString(const vector<unsigned char>& data){
    return string(data.begin(),data.end());
}
int chooseAlgorithm(){
    int choice;
    cout << "\nВыберите алгоритм:\n\n";
    cout << "1. Аффинный шифр\n";
    cout << "2. Шифр Хилла\n";
    cout << "3. RSA (в разработке)\n";
    cout << "4. Диффи-Хеллман (в разработке)\n";
    cout << "5. Шифр Шамира (в разработке)\n";
    cout << "6. Шифр Эль-Гамаля (в разработке)\n";
    cout << "\n0. Назад\n\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    return choice;
}
void keyGeneratorMenu(){
    int choice;
    cout << "\nГенератор ключей\n\n";
    cout << "1. Аффинный шифр\n";
    cout << "2. Шифр Хилла\n";
    cout << "\n0. Назад\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    
    switch (choice){
        case 1:{
            AffineKey key = generateAffineKey();
            cout << "\nСгенерированный ключ:\n";
            cout << "a = " << key.a << endl;
            cout << "b = " << key.b << endl;
            break;
        }

        case 2:{
            auto key = generateHillKey();
            cout << "\nМатрица:\n";
            for (const auto& row : key){
                for (int value : row){
                    cout << value << '\t';
                }
                cout << endl;
            }
            break;
        }
        default:break;
    }

    waitForEnter();
}
vector<unsigned char> processAffine(const vector<unsigned char>& data, bool encryptMode){
    int a;int b;
    cout << "\nВведите ключ a: ";
    cin >> a;
    cout << "Введите ключ b: ";
    cin >> b;
    if (gcd(a, 256) != 1){
        throw runtime_error("Ключ a должен быть взаимно простым с 256.");
    }

    if (encryptMode){
        return encryptAffine(data,a, b);
    }

    return decryptAffine(data, a, b);
}
vector<unsigned char> processHill(const vector<unsigned char>& data, bool encryptMode){
    vector<vector<int>> key(2, vector<int>(2));
    cout << "\nВведите матрицу 2x2:\n";
    for (auto& row : key){
        for (auto& value : row){
            cin >> value;
        }
    }
    int determinant = key[0][0] * key[1][1] - key[0][1] * key[1][0];
    determinant %= 256;
    if (determinant < 0){
        determinant += 256;
    }
    if (gcd(determinant, 256) != 1){
        throw runtime_error("Матрица не имеет обратной по mod 256.");
    }
    if (encryptMode){
        return encryptHill(data, key);
    }
    return decryptHill(data, key);
}
void fileMenu(){
    int algorithm = chooseAlgorithm();
    if (algorithm == 0){
        return;
    }
    if (algorithm >= 3){
        printError("Алгоритм пока не реализован.");
        waitForEnter();
        return;
    }
    int mode;
    cout << "\n1. Шифрование\n" << "2. Дешифрование\n" << "\n0. Назад\n\n";
    cout << "Ваш выбор: ";
    cin >> mode;
    if (mode == 0){
        return;
    }
    string inputFile;
    cout<< "\nВведите имя файла:\n";
    cin >> inputFile;
    vector<unsigned char> data = readFile(inputFile);
    vector<unsigned char> result;
    if (algorithm == 1){
        result = processAffine(data,mode == 1);
    }
    else{
        result = processHill(data, mode == 1);
    }
    string outputFile;
    if (mode == 1){
        outputFile = "encrypted_" + inputFile;
    }
    else{
        outputFile = "decrypted_" + inputFile;
    }
    writeFile(outputFile,result);
    printSuccess("Файл сохранён как:\n" + outputFile);
    waitForEnter();
}
void textMenu(){
    int algorithm = chooseAlgorithm();
    if (algorithm == 0){
        return;
    }
    if (algorithm >= 3){
        printError("Алгоритм пока не реализован.");
        waitForEnter();
        return;
    }
    int mode;
    cout << "\n1. Шифрование\n";
    cout << "2. Дешифрование\n";
    cout << "\n0. Назад\n\n";
    cout << "Ваш выбор: ";
    cin >> mode;
    if (mode == 0){
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string text;
    if (mode == 1){
        cout << "\nВведите текст:\n";
    }
    else{
        cout << "\nВведите HEX:\n";
    }
    getline(cin, text);
    vector<unsigned char> data;
    if (mode == 1){
        data = stringToBytes(text);
    }
    else{
        data = hexToBytes(text);
    }
    vector<unsigned char> result;
    if (algorithm == 1){
        result = processAffine(data,mode == 1);
    }
    else{
        result = processHill(data,mode == 1);
    }

    if (mode == 1){
        cout << "\nРезультат (HEX):\n";
        for (unsigned char byte : result){
            printf("%02X", byte);
        }
        cout << endl;
    }
    else{
        cout << "\nРезультат:\n";
        cout << bytesToString(result) << endl;
    }

    waitForEnter();
}

int main(){
    try{
        while (true){
            int choice;
            cout << "\n=======================\n";
            cout << "Система шифрования данных\n";
            cout << "=========================\n\n";
            cout << "1. Работа с текстом\n";
            cout << "2. Работа с файлами\n";
            cout << "3. Генератор ключей\n";
            cout << "\n0. Выход\n\n";
            cout << "Ваш выбор: ";
            cin >> choice;

            switch (choice){
                case 1:
                    textMenu();
                    break;
                case 2:
                    fileMenu();
                    break;
                case 3:
                    keyGeneratorMenu();
                    break;
                case 0:
                    cout << "\nЗавершение программы...\n";
                    return 0;
                default:
                    printError("Некорректный выбор.");
                    waitForEnter();
                    break;
            }
        }
    }
    catch (const exception& ex){
        printError(ex.what());
        waitForEnter();
    }
    catch (...){
        printError("Неизвестная ошибка.");
        waitForEnter();
    }
    return 0;
}
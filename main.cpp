#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cstdio>
#include "sencipher.h"
#include "hillcipher.h"
#include "keygenerator.h"
#include "publickeycipher.h"
using namespace std;
using namespace SenCipher;

enum class MainMenu{
    Exit = 0,
    Text = 1,
    File = 2,
    KeyGenerator = 3
};

enum class Algorithm{
    Back = 0,
    Affine = 1,
    Hill = 2,
    RSA = 3,
    DiffieHellman = 4,
    Shamir = 5,
    ElGamal = 6
};

enum class Mode{
    Back = 0,
    Encrypt = 1,
    Decrypt = 2
};
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

int showMenu(const string& title,
             const vector<string>& items,
             bool showBack = true){

    cout << "\n====================================\n";
    cout << title << '\n';
    cout << "====================================\n\n";

    int number = 1;

    for (const auto& item : items){
        cout << number++ << ". " << item << '\n';
    }

    if (showBack){
        cout << "\n0. Назад\n";
    }

    cout << "\nВаш выбор: ";

    int choice;
    cin >> choice;

    return choice;
}

Algorithm chooseAlgorithm(){

    return static_cast<Algorithm>(
        showMenu(
            "Выберите алгоритм",
            {
                "Аффинный шифр",
                "Шифр Хилла",
                "RSA (в разработке)",
                "Диффи-Хеллман (в разработке)",
                "Шифр Шамира",
                "Шифр Эль-Гамаля"
            }
        )
    );
}
void keyGeneratorMenu(){
    int choice;
    cout << "\nГенератор ключей\n\n";
    cout << "1. Аффинный шифр\n";
    cout << "2. Шифр Хилла\n";
    cout << "3. Шифр Шамира\n";
    cout << "4. Шифр Эль-Гамаля\n";
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
        case 3:{
            int p;
            cout << "\nВведите простое p больше 255 (например 257): ";
            cin >> p;
            ShamirKeys keys = generateShamirKeys(p);
            cout << "\nКлючи Шамира:\n";
            cout << "p = " << keys.p << endl;
            cout << "Алиса: cA = " << keys.alice.c << ", dA = " << keys.alice.d << endl;
            cout << "Боб:   cB = " << keys.bob.c << ", dB = " << keys.bob.d << endl;
            break;
        }
        case 4:{
            int p; int g;
            cout << "\nВведите простое p больше 255 (например 257): ";
            cin >> p;
            cout << "Введите g от 2 до p - 1 (например 3): ";
            cin >> g;
            ElGamalKey key = generateElGamalKey(p, g);
            cout << "\nКлючи Эль-Гамаля:\n";
            cout << "Открытый ключ: p = " << key.p << ", g = " << key.g << ", y = " << key.y << endl;
            cout << "Закрытый ключ: x = " << key.x << endl;
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

vector<unsigned char> processShamir(const vector<unsigned char>& data, bool encryptMode){
    int p;
    cout << "\nВведите простое p больше 255: ";
    cin >> p;

    if (encryptMode){
        int ca; int da; int cb;
        cout << "Введите ключ Алисы cA: ";
        cin >> ca;
        cout << "Введите обратный ключ Алисы dA: ";
        cin >> da;
        cout << "Введите ключ Боба cB: ";
        cin >> cb;

        return encryptShamir(data, p, ca, da, cb);
    }

    int db;
    cout << "Введите обратный ключ Боба dB: ";
    cin >> db;
    return decryptShamir(data, p, db);
}

vector<unsigned char> processElGamal(const vector<unsigned char>& data, bool encryptMode){
    int p;
    cout << "\nВведите простое p больше 255: ";
    cin >> p;

    if (encryptMode){
        int g; int y;
        cout << "Введите основание g: ";
        cin >> g;
        cout << "Введите открытый ключ y: ";
        cin >> y;

        return encryptElGamal(data, p, g, y);
    }

    int x;
    cout << "Введите закрытый ключ x: ";
    cin >> x;
    return decryptElGamal(data, p, x);
}

void fileMenu(){
    Algorithm algorithm = chooseAlgorithm();
    if (algorithm == Algorithm::Back){
        return;
    }

    if (algorithm == Algorithm::RSA ||
        algorithm == Algorithm::DiffieHellman)
    {
        printError("Алгоритм пока не реализован.");
        waitForEnter();
        return;
    }
    Mode mode = static_cast<Mode>(showMenu("Выберите режим",{
        "Шифрование",
        "Дешифрование"
        }));
    if (mode == Mode::Back){
        return;
    }
    string inputFile;
    cout<< "\nВведите имя файла:\n";
    cin >> inputFile;
    vector<unsigned char> data = readFile(inputFile);
    vector<unsigned char> result;
    if (algorithm == Algorithm::Affine){
    result = processAffine(data, mode == Mode::Encrypt);
    }
    else if (algorithm == Algorithm::Hill){
        result = processHill(data, mode == Mode::Encrypt);
    }
    else if (algorithm == Algorithm::Shamir){
        result = processShamir(data, mode == Mode::Encrypt);
    }
    else if (algorithm == Algorithm::ElGamal){
        result = processElGamal(data, mode == Mode::Encrypt);
    }   
    string outputFile;
    if (mode == Mode::Encrypt){
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
    Algorithm algorithm = chooseAlgorithm();
    if (algorithm == Algorithm::Back){
        return;
    }
    if (algorithm == Algorithm::RSA ||
        algorithm == Algorithm::DiffieHellman){
            printError("Алгоритм пока не реализован.");
            waitForEnter();
            return;
        }
    Mode mode = static_cast<Mode>(
    showMenu("Выберите режим",{
            "Шифрование",
            "Дешифрование"
        }));
    if (mode == Mode::Back){
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string text;
    if (mode == Mode::Encrypt){
        cout << "\nВведите текст:\n";
    }
    else{
        cout << "\nВведите HEX:\n";
    }
    getline(cin, text);
    vector<unsigned char> data;
    if (mode == Mode::Encrypt){
        data = stringToBytes(text);
    }
    else{
        data = hexToBytes(text);
    }
    vector<unsigned char> result;
    if (algorithm == Algorithm::Affine){
        result = processAffine(data, mode == Mode::Encrypt);
    }
    else if (algorithm == Algorithm::Hill){
        result = processHill(data, mode == Mode::Encrypt);
    }
    else if (algorithm == Algorithm::Shamir){
        result = processShamir(data, mode == Mode::Encrypt);
    }
    else if (algorithm == Algorithm::ElGamal){
        result = processElGamal(data, mode == Mode::Encrypt);
    }

    if (mode == Mode::Encrypt){
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
            MainMenu choice = static_cast<MainMenu>(showMenu(
            "Система шифрования данных",{
                "Работа с текстом",
                "Работа с файлами",
                "Генератор ключей"
            }));
            switch (choice){
                case MainMenu::Text:
                textMenu();
                break;

            case MainMenu::File:
                fileMenu();
                break;

            case MainMenu::KeyGenerator:
                keyGeneratorMenu();
                break;

            case MainMenu::Exit:
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

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
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
void keyGeneratorMenu()
{
    int choice;

    cout << "\nГенератор ключей\n\n";

    cout << "1. Аффинный шифр\n";
    cout << "2. Шифр Хилла\n";

    cout << "\n0. Назад\n";

    cout << "Ваш выбор: ";

    cin >> choice;
// поправить в дефолтный вид
    /* switch (choice) 
    {
        case 1:
        {
            AffineKey key =
                generateAffineKey();

            cout << "\nСгенерированный ключ:\n";

            cout
                << "a = "
                << key.a
                << endl;

            cout
                << "b = "
                << key.b
                << endl;

            break;
        }

        case 2:
        {
            auto key =
                generateHillKey();

            cout << "\nМатрица:\n";

            for (const auto& row : key)
            {
                for (int value : row)
                {
                    cout
                        << value
                        << '\t';
                }

                cout << endl;
            }

            break;
        }

        default:
            break;
    }

    waitForEnter();
} */

#include "cipherRailFence.h"
#include <stdexcept>
#include <windows.h> //добавил для функций конвертации кодировок

using namespace std;

//функия конвертирует  UTF-8 байты в широкие символы
wstring bytesToWstring(const vector<unsigned char>& data) {
    if (data.empty()) return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, (const char*)data.data(), data.size(), NULL, 0);
    wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, (const char*)data.data(), data.size(), &result[0], size);
    return result;
}

//функция возвращает широкие символы обратно в UTF-8 байты
vector<unsigned char> wstringToBytes(const wstring& wstr) {
    if (wstr.empty()) return {};
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), NULL, 0, NULL, NULL);
    vector<unsigned char> result(size);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), (char*)result.data(), size, NULL, NULL);
    return result;
}

vector<unsigned char> CipherRailFence::encrypt(const vector<unsigned char>& data, const string& key) {
    if (data.empty()) return data;
    
    int numRails = 0;
    try {
        numRails = stoi(key);
    } catch (...) {
        throw invalid_argument("Ключ для шифра Зигзаг должен быть целым числом!");
    }
    if (numRails <= 1) throw invalid_argument("Количество строк должно быть больше 1!");

    //переводим байты в неделимые широкие символы
    wstring wdata = bytesToWstring(data);
    if (wdata.empty()) return data;

    vector<vector<wchar_t>> fence(numRails);
    int rail = 0;
    bool directionDown = false;

    //переставляем целые символы, а не отдельные байты
    for (size_t i = 0; i < wdata.size(); ++i) {
        fence[rail].push_back(wdata[i]);
        if (rail == 0 || rail == numRails - 1) {
            directionDown = !directionDown;
        }
        rail += directionDown ? 1 : -1;
    }

    wstring wresult;
    wresult.reserve(wdata.size());
    for (int i = 0; i < numRails; ++i) {
        for (size_t j = 0; j < fence[i].size(); ++j) {
            wresult.push_back(fence[i][j]);
        }
    }
    return wstringToBytes(wresult);
}

vector<unsigned char> CipherRailFence::decrypt(const vector<unsigned char>& data, const string& key) {
    if (data.empty()) return data;

    int numRails = 0;
    try {
        numRails = stoi(key);
    } catch (...) {
        throw invalid_argument("Ключ для шифра Зигзаг должен быть целым числом");
    }
    if (numRails <= 1) throw invalid_argument("Количество строк должно быть больше 1");

    wstring wdata = bytesToWstring(data);
    if (wdata.empty()) return data;

    vector<vector<bool>> marker(numRails, vector<bool>(wdata.size(), false));
    int rail = 0;
    bool directionDown = false;

    for (size_t i = 0; i < wdata.size(); ++i) {
        marker[rail][i] = true;
        if (rail == 0 || rail == numRails - 1) {
            directionDown = !directionDown;
        }
        rail += directionDown ? 1 : -1;
    }

    vector<vector<wchar_t>> fence(numRails);
    size_t dataIndex = 0;
    for (int i = 0; i < numRails; ++i) {
        for (size_t j = 0; j < wdata.size(); ++j) {
            if (marker[i][j] && dataIndex < wdata.size()) {
                fence[i].push_back(wdata[dataIndex++]);
            }
        }
    }

    wstring wresult;
    wresult.reserve(wdata.size());
    vector<size_t> rIndex(numRails, 0);
    rail = 0;
    directionDown = false;

    for (size_t i = 0; i < wdata.size(); ++i) {
        wresult.push_back(fence[rail][rIndex[rail]++]);
        if (rail == 0 || rail == numRails - 1) {
            directionDown = !directionDown;
        }
        rail += directionDown ? 1 : -1;
    }
    return wstringToBytes(wresult);
}

CipherAPI* createCipher() { return new CipherRailFence(); }
void releaseCipher(CipherAPI* cipher) { delete cipher; }

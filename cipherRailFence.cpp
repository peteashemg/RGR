#include "cipherRailFence.h"
#include <stdexcept>
#include <windows.h>

std::wstring bytesToWstring(const std::vector<unsigned char>& data) {
    if (data.empty()) return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, (const char*)data.data(), data.size(), NULL, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, (const char*)data.data(), data.size(), result.data(), size);
    return result;
}

std::vector<unsigned char> wstringToBytes(const std::wstring& wstr) {
    if (wstr.empty()) return {};
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), NULL, 0, NULL, NULL);
    std::vector<unsigned char> result(size);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), (char*)result.data(), size, NULL, NULL);
    return result;
}

std::vector<unsigned char> CipherRailFence::encrypt(const std::vector<unsigned char>& data, const std::string& key) {
    if (data.empty()) return data;
    
    int numRails = 0;
    try {
        numRails = std::stoi(key);
    } catch (...) {
        throw std::invalid_argument("Ключ для шифра Зигзаг должен быть целым числом");
    }
    if (numRails <= 1) throw std::invalid_argument("Количество строк должно быть больше 1");

    std::wstring wdata = bytesToWstring(data);
    if (wdata.empty()) return data;

    std::vector<std::vector<wchar_t>> fence(numRails);
    int rail = 0;
    bool directionDown = false;

    for (size_t i = 0; i < wdata.size(); ++i) {
        fence[rail].push_back(wdata[i]);
        if (rail == 0 || rail == numRails - 1) {
            directionDown = !directionDown;
        }
        rail += directionDown ? 1 : -1;
    }

    std::wstring wresult;
    wresult.reserve(wdata.size());
    for (int i = 0; i < numRails; ++i) {
        for (size_t j = 0; j < fence[i].size(); ++j) {
            wresult.push_back(fence[i][j]);
        }
    }
    return wstringToBytes(wresult);
}

std::vector<unsigned char> CipherRailFence::decrypt(const std::vector<unsigned char>& data, const std::string& key) {
    if (data.empty()) return data;

    int numRails = 0;
    try {
        numRails = std::stoi(key);
    } catch (...) {
        throw std::invalid_argument("Ключ для шифра Зигзаг должен быть целым числом");
    }
    if (numRails <= 1) throw std::invalid_argument("Количество строк должно быть больше 1");

    std::wstring wdata = bytesToWstring(data);
    if (wdata.empty()) return data;

    std::vector<std::vector<bool>> marker(numRails, std::vector<bool>(wdata.size(), false));
    int rail = 0;
    bool directionDown = false;

    for (size_t i = 0; i < wdata.size(); ++i) {
        marker[rail][i] = true;
        if (rail == 0 || rail == numRails - 1) {
            directionDown = !directionDown;
        }
        rail += directionDown ? 1 : -1;
    }

    std::vector<std::vector<wchar_t>> fence(numRails);
    size_t dataIndex = 0;
    for (int i = 0; i < numRails; ++i) {
        for (size_t j = 0; j < wdata.size(); ++j) {
            if (marker[i][j] && dataIndex < wdata.size()) {
                fence[i].push_back(wdata[dataIndex++]);
            }
        }
    }

    std::wstring wresult;
    wresult.reserve(wdata.size());
    std::vector<size_t> rIndex(numRails, 0);
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

<<<<<<< HEAD
#pragma once

#include <windows.h>
#include <string>
#include "cipherapi.h"

typedef CipherAPI* (*CreateCipherFunc)();
typedef void (*ReleaseCipherFunc)(CipherAPI*);

class PluginLoader {
private:
    HMODULE dllHandle;

public:
    PluginLoader();
    ~PluginLoader();

    CipherAPI* loadCipher(
        const std::string& dllName,
        ReleaseCipherFunc& releaseFunc);
};
=======
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H
#include <windows.h>
#include <string>

class PluginLoader{
private: HMODULE handle;
public: PluginLoader();
    bool load(const std::string& path);
    void unload();
    void* getFunction(const std::string& name);
    ~PluginLoader();
};

#endif
>>>>>>> 93828e04832088829754561e1cce934810836682

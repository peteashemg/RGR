#include "pluginloader.h"

PluginLoader::PluginLoader() {
    dllHandle = nullptr;
}

PluginLoader::~PluginLoader() {
    if (dllHandle)
        FreeLibrary(dllHandle);
}

CipherAPI* PluginLoader::loadCipher(
    const std::string& dllName,
    ReleaseCipherFunc& releaseFunc)
{
    dllHandle = LoadLibraryA(dllName.c_str());

    if (!dllHandle)
        return nullptr;

    CreateCipherFunc createFunc =
        (CreateCipherFunc)GetProcAddress(
            dllHandle,
            "createCipher");

    releaseFunc =
        (ReleaseCipherFunc)GetProcAddress(
            dllHandle,
            "releaseCipher");

    if (!createFunc || !releaseFunc)
        return nullptr;

    return createFunc();
}
#include "pluginloader.h"

<<<<<<< HEAD
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
=======
PluginLoader::PluginLoader()
    : handle(nullptr){}

bool PluginLoader::load(
    const std::string& path){
    handle = LoadLibraryA(path.c_str());
    return handle != nullptr;
}
void* PluginLoader::getFunction(const std::string& name){
    if (!handle){
        return nullptr;
    }
    return reinterpret_cast<void*>(GetProcAddress(handle, name.c_str()));
}
void PluginLoader::unload(){
    if (handle){
        FreeLibrary(handle);
        handle = nullptr;
    }
}
PluginLoader::~PluginLoader(){
    unload();
>>>>>>> 93828e04832088829754561e1cce934810836682
}
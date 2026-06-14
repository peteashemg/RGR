#include "pluginloader.h"

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
}
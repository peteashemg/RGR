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
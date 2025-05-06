#include "NeoCCAMS.h"

extern "C" PLUGIN_API PluginSDK::BasePlugin *CreatePluginInstance()
{
    try
    {
        return new NeoCCAMS();
    }
    catch (const std::exception &e)
    {
        return nullptr;
    }
}
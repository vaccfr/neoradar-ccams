// NeoCCAMS.cpp
#ifndef PLUGIN_VERSION
#define PLUGIN_VERSION "no_version"
#endif
#include "NeoCCAMS.h"
#include "SquawkProvider.h"

NeoCCAMS::NeoCCAMS() = default;
NeoCCAMS::~NeoCCAMS() = default;

void NeoCCAMS::Initialize(const PluginSDK::PluginMetadata &metadata, PluginSDK::CoreAPI *coreAPI, PluginSDK::ClientInformation info)
{
    metadata_ = metadata;
    clientInfo_ = info;
    coreAPI_ = coreAPI;
    fsdAPI_ = &coreAPI_->fsd();
    aircraftAPI_ = &coreAPI_->aircraft();
    controllerDataAPI_ = &coreAPI_->controllerData();
    logger_ = &coreAPI_->logger();

    logger_->info("Initializing NeoCAMS" + metadata.version);
    try
    {
        squawkProvider = std::make_shared<Squawk::SquawkProvider>(info, *fsdAPI_, *aircraftAPI_, *controllerDataAPI_, *logger_);
        initialized_ = coreAPI_->squawk().registerProvider(squawkProvider);;
        logger_->info("NeoCCAMS initialized successfully");
    }
    catch (const std::exception &e)
    {
        logger_->error("Failed to initialize NeoCCAMS: " + std::string(e.what()));
    }
}

void NeoCCAMS::Shutdown()
{
    if (initialized_)
    {
        squawkProvider.reset();
        initialized_ = false;
        logger_->info("NeoCCAMS shutdown complete");
    }
    
}

PluginSDK::PluginMetadata NeoCCAMS::GetMetadata() const
{
    return {"NeoCCAMS", PLUGIN_VERSION, "French VACC"};
}
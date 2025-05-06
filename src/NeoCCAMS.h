// NeoCCAMS.h
#pragma once
#include <memory>
#include "SDK.h"
#include "SquawkProvider.h"

class NeoCCAMS : public PluginSDK::BasePlugin
{
public:
    NeoCCAMS();
    ~NeoCCAMS();

    void Initialize(const PluginSDK::PluginMetadata &metadata, PluginSDK::CoreAPI *coreAPI, PluginSDK::ClientInformation info) override;
    void Shutdown() override;
    PluginSDK::PluginMetadata GetMetadata() const override;

    // Squawk provider
    std::shared_ptr<Squawk::SquawkProvider> squawkProvider;

private:
    bool initialized_ = false;
    PluginSDK::PluginMetadata metadata_;
    PluginSDK::ClientInformation clientInfo_;
    PluginSDK::CoreAPI *coreAPI_ = nullptr;
    PluginSDK::Fsd::FsdAPI *fsdAPI_ = nullptr;
    PluginSDK::Aircraft::AircraftAPI *aircraftAPI_ = nullptr;
    PluginSDK::ControllerData::ControllerDataAPI *controllerDataAPI_ = nullptr;
    PluginSDK::Logger::LoggerAPI *logger_ = nullptr;

};
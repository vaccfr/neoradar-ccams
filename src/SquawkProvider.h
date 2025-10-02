// SquawkProvider.h
#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <mutex>
#include <random>
#include <regex>
#include <NeoRadarSDK/SDK.h>

namespace Squawk
{

    const std::regex MODES_AIRPORT_ICAO_PATTERN("^((E([BDHLT]|P(?!CE|DA|DE|IR|KS|LK|LY|MB|MI|MM|OK|PR|PW|SN|TM)|URM)|L[DHIKORZ])[A-Z]{2}|LF[^V][A-Z]|LS(G[CG]|Z[BGHR]))");
    const std::regex MODES_EQUIPMENT_PATTERN("[EHILPSX]");
    const std::string MODES_SQUAWK = "1000";
    const std::string VFR_SQUAWK = "7000";
    const std::string CCAMS_API_BASE = "https://ccams.kilojuliett.ch";
    const std::string CCAMS_API_ENDPOINT = "/squawk";
    

    class SquawkProvider : public PluginSDK::Squawk::SquawkProviderInterface
    {
    public:
        SquawkProvider(
            PluginSDK::ClientInformation info,
            PluginSDK::Fsd::FsdAPI &fsdAPI,
            PluginSDK::Aircraft::AircraftAPI &aircraftAPI,
            PluginSDK::ControllerData::ControllerDataAPI &fcontrollerDataAPI,
            PluginSDK::Logger::LoggerAPI &logger);

        // Implement SquawkProviderInterface methods
        std::string GenerateSquawk(
            const std::string &callsign,
            const PluginSDK::Aircraft::Aircraft &aircraft,
            const PluginSDK::Flightplan::Flightplan &flightplan) override;

        std::string GetProviderName() const override;

        int GetPriority() const override;

    private:
        PluginSDK::Fsd::FsdAPI &fsdAPI_;
        PluginSDK::Aircraft::AircraftAPI &aircraftAPI_;
        PluginSDK::ControllerData::ControllerDataAPI &controllerDataAPI_;
        PluginSDK::ClientInformation info_;
        PluginSDK::Logger::LoggerAPI &logger_;
        std::unique_ptr<httplib::Client> httpClient_;
        std::vector<std::string> assignedSquawks_;
        std::mutex assignedSquawksMutex_;

        // Random number generator for fallback squawks
        std::mt19937 rng_;
        std::uniform_int_distribution<int> dist_;

        // Generate fallback squawk when API fails
        std::string GenerateFallbackSquawk();

        bool isModeS(
            const std::string &origin,
            const std::string &destination,
            const std::string &transponderEquipment);
        std::string collectSquawks();
        std::string trim(const std::string & source);
    };
}
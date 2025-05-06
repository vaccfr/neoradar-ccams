// SquawkProvider.cpp
#include <regex>
#include <sstream>
#include "SquawkProvider.h"

namespace Squawk
{

    SquawkProvider::SquawkProvider(
        PluginSDK::ClientInformation info,
        PluginSDK::Fsd::FsdAPI &fsdAPI,
        PluginSDK::Aircraft::AircraftAPI &aircraftAPI,
        PluginSDK::ControllerData::ControllerDataAPI &controllerDataAPI,
        PluginSDK::Logger::LoggerAPI &logger) : fsdAPI_(fsdAPI),
                                                aircraftAPI_(aircraftAPI),
                                                controllerDataAPI_(controllerDataAPI),
                                                info_(info),
                                                logger_(logger),
                                                rng_(std::random_device{}()),
                                                dist_(0, 7)
    {
        // Build SSL Client
        std::string host = GetHost(CCAMS_API_BASE);
        logger_.info("Creating HTTPS client for host: " + host);
        httpsClient_ = std::make_unique<httplib::SSLClient>(host);
        httpsClient_->set_connection_timeout(10); // 10 seconds
        httpsClient_->set_read_timeout(30);       // 30 seconds
        httpsClient_->set_write_timeout(30);      // 30 seconds
        // Skip verification in development (remove in production)
        httpsClient_->enable_server_certificate_verification(true);

    }

    std::string SquawkProvider::GenerateSquawk(
        const std::string &callsign,
        const PluginSDK::Aircraft::Aircraft &aircraft,
        const PluginSDK::Flightplan::Flightplan &flightplan)
    {
        logger_.info("NeoCAMS provider generating squawk for " + callsign + " from " + flightplan.origin + " to " + flightplan.destination + " with equipment " + flightplan.transponderEquipment);
       
        if (flightplan.flightRule == "V")
        {
            logger_.info("Squawk VFR for " + callsign + ": " + VFR_SQUAWK);
            return VFR_SQUAWK;
        }
        
        if (isModeS(flightplan.origin, flightplan.destination, flightplan.transponderEquipment))
        {
            logger_.info("Squawk Mode S for " + callsign + ": " + MODES_SQUAWK);
            return MODES_SQUAWK;
        }

        if (fsdAPI_.getConnection().has_value()) 
        {
            std::string url = CCAMS_API_BASE + CCAMS_API_ENDPOINT + 
            "?callsign=" + fsdAPI_.getConnection().value().callsign +
            "&origin=" + flightplan.origin +
            "&dest=" + flightplan.destination +
            "&flightrule=I" +
            "&connectiontype=1";

            
            std::string squawks = collectSquawks();
            if (!squawks.empty())
            {
                url += "&codes=" + squawks;
            }

            PluginSDK::Fsd::ServerType serverType = fsdAPI_.getConnection().value().serverType;
            if (serverType == PluginSDK::Fsd::ServerType::Test || serverType == PluginSDK::Fsd::ServerType::Sweatbox)
            {
                url += "&sim";
            }

            httplib::Headers headers;
            httplib::Result result;
            headers.emplace("User-Agent", "neoradar/" + info_.clientVersion);
            try
            {
                result = httpsClient_->Get(url.c_str(), headers);
            }
            catch (const std::exception &e)
            {
                logger_.error("Error while calling CCAMS API: " + std::string(e.what()));
                return GenerateFallbackSquawk();
            }

            if (result->status != 200)
            {
                logger_.error("CCAMS API returned error: " + std::to_string(result->status));
                return GenerateFallbackSquawk();
            }

            logger_.info("Recieved squawk " + trim(result->body) + " for " + callsign);
            return trim(result->body);
        }

        // If all else fails, generate a fallback squawk
        std::string fallbackSquawk = GenerateFallbackSquawk();
        logger_.info("Using fallback squawk for " + callsign + ": " + fallbackSquawk);
        return fallbackSquawk;
    }

    std::string SquawkProvider::GetProviderName() const
    {
        return "Neo Radar CCAMS Squawk Provider";
    }

    int SquawkProvider::GetPriority() const
    {
        return 100; // Higher than default (0)
    }

    std::string SquawkProvider::GenerateFallbackSquawk()
    {
        // Generate a random valid squawk code
        std::stringstream squawk;
        for (int i = 0; i < 4; ++i)
        {
            squawk << dist_(rng_);
        }
        return squawk.str();
    }

    std::string SquawkProvider::GetHost(const std::string url) const
    {
        std::string host = url;

        // Remove protocol prefix if present
        size_t protocolPos = host.find("://");
        if (protocolPos != std::string::npos) {
            host = host.substr(protocolPos + 3);
        }

        // Remove path if present
        size_t pathPos = host.find('/');
        if (pathPos != std::string::npos) {
            host = host.substr(0, pathPos);
        }

        return host;
    }

    bool SquawkProvider::isModeS(
        const std::string &origin,
        const std::string &destination,    
        const std::string &transponderEquipment
    ) 
    {
        return std::regex_match(origin, MODES_AIRPORT_ICAO_PATTERN) &&
               std::regex_match(destination, MODES_AIRPORT_ICAO_PATTERN) &&
               std::regex_search(transponderEquipment, MODES_EQUIPMENT_PATTERN);
    }

    std::string SquawkProvider::collectSquawks()
    {
        std::vector<std::string> collectedSquawks;
        std::vector<PluginSDK::Aircraft::Aircraft> aircrafts = aircraftAPI_.getAll();
        for (const auto &aircraft : aircrafts)
        {
            if (aircraft.squawk != "0000" && aircraft.squawk != VFR_SQUAWK && aircraft.squawk != MODES_SQUAWK)
            {
                if (std::find(collectedSquawks.begin(), collectedSquawks.end(), aircraft.squawk) == collectedSquawks.end())
                {
                    collectedSquawks.push_back(aircraft.squawk);
                }
            }
        }
        std::vector<PluginSDK::ControllerData::ControllerDataModel> controllerData = controllerDataAPI_.getAll();
        for (const auto &data : controllerData)
        {
            if (data.assignedSquawk != "0000" && data.assignedSquawk != VFR_SQUAWK && data.assignedSquawk != MODES_SQUAWK)
            {
                if (std::find(collectedSquawks.begin(), collectedSquawks.end(), data.assignedSquawk) == collectedSquawks.end())
                {
                    collectedSquawks.push_back(data.assignedSquawk);
                }
            }
        }
        if (!collectedSquawks.empty())
        {
            std::string squawks = "";
            for (size_t i = 0; i < collectedSquawks.size(); ++i)
            {
                squawks += collectedSquawks[i];
                if (i != collectedSquawks.size() - 1)
                {
                    squawks += ",";
                }
            }
            return squawks;
        }
        return "";
    }
    
    std::string SquawkProvider::trim(const std::string & source) {
        std::string s(source);
        s.erase(0,s.find_first_not_of(" \n\r\t"));
        s.erase(s.find_last_not_of(" \n\r\t")+1);
        return s;
    }
}
#pragma once
#include <string>
#include <vector>
#include <optional>

namespace PluginSDK::Flightplan {

/**
 * @enum VoiceType
 * @brief Type of voice communication
 */
enum class VoiceType {
    Unknown = 0,
    Full = 1,
    Receive = 2,
    Text = 3
};

/**
 * @struct Flightplan
 * @brief Represents a stored flightplan in the system
 */
struct Flightplan {
    std::string callsign;
    std::string flightRule;
    std::string rawType;
    std::string acType; // Short aircraft type (e.g., "B738")
    std::string equipment; // ICAO equipment
    std::string wakeCategory; // Wake turbulence category
    std::string transponderEquipment; // ICAO transponder equipment
    std::string origin;
    std::string destination;
    std::string alternate;
    std::string route;
    int plannedAltitude;
    int plannedTas;
    std::string flightTimeHours;
    std::string flightTimeMinutes;
    std::string fuelTimeHours;
    std::string fuelTimeMinutes;
    std::string eobt;
    std::string aobt;
    std::string remarks;
    bool isValid = true;
    VoiceType voiceType;
};
/**
 * @struct FlightplanUpdatedEvent
 * @brief Event fired when a flightplan is updated
 */
struct FlightplanUpdatedEvent {
    std::string callsign;
    std::string origin;
    std::string destination;
    std::string route;
    std::string acType;
    std::string altitude;
    std::string rules;
};

/**
 * @struct FlightplanRemovedEvent
 * @brief Event fired when a flightplan is removed
 */
struct FlightplanRemovedEvent {
    std::string callsign;
};

/**
 * @struct FlightplanVoiceTypeChangedEvent
 * @brief Event fired when a flightplan's voice type changes
 */
struct FlightplanVoiceTypeChangedEvent {
    std::string callsign;
    VoiceType oldVoiceType;
    VoiceType newVoiceType;
};

/**
 * @struct FlightplanRouteChangedEvent
 * @brief Event fired when a flightplan's route changes
 */
struct FlightplanRouteChangedEvent {
    std::string callsign;
    std::string newRoute;
};

/**
 * @interface FlightplanAPI
 * @brief Interface for flightplan operations
 */
class FlightplanAPI {
public:
    virtual ~FlightplanAPI() = default;

    /**
     * @brief Get all flightplans in the system
     * @return Vector of flightplan data
     */
    virtual std::vector<Flightplan> getAll() = 0;

    /**
     * @brief Get a flightplan by callsign
     * @param callsign The callsign to look up
     * @return Flightplan data or nullptr if not found
     */
    virtual std::optional<Flightplan> getByCallsign(const std::string& callsign) = 0;
};

} // namespace PluginSDK::Flightplan
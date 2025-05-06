// plugin/sdk/TextMessage.h
#pragma once
#include <string>
#include <vector>

namespace PluginSDK::TextMessage {

/**
 * @enum RequestType
 * @brief Type of flightplan request
 */
enum class RequestType {
    Unknown = 0,
    Clearance = 1,
    Pushback = 2,
    TaxiOut = 3,
    TaxiIn = 4,
    Startup = 5
};

/**
 * @struct FrequencyMessageReceivedEvent
 * @brief Event fired when a frequency message is received
 */
struct FrequencyMessageReceivedEvent {
    std::string sentFrom;
    std::string message;
    std::vector<int> frequencies;
    bool fromMe;
};

/**
 * @struct FlightplanMessageReceivedEvent
 * @brief Event fired when a flightplan message is received
 */
struct FlightplanMessageReceivedEvent {
    RequestType requestType;
    std::string callsign;
    std::string request;
};

/**
 * @struct ATISInfoMessageReceivedEvent
 * @brief Event fired when an ATIS info message is received
 */
struct ATISInfoMessageReceivedEvent {
    std::string callsign;
    std::string icao;
    std::string atisLetter;
};

/**
 * @struct PrivateMessageReceivedEvent
 * @brief Event fired when a private message is received
 */
struct PrivateMessageReceivedEvent {
    std::string sentFrom;
    std::string sentTo;
    std::string message;
    bool fromMe;
};

/**
 * @struct BroadcastMessageReceivedEvent
 * @brief Event fired when a broadcast message is received
 */
struct BroadcastMessageReceivedEvent {
    std::string sentFrom;
    std::string message;
    bool fromMe;
};

/**
 * @struct SupervisorMessageReceivedEvent
 * @brief Event fired when a supervisor message is received
 */
struct SupervisorMessageReceivedEvent {
    std::string sentFrom;
    std::string message;
    bool fromMe;
};

/**
 * @struct ServerMessageReceivedEvent
 * @brief Event fired when a server message is received
 */
struct ServerMessageReceivedEvent {
    std::string sentFrom;
    std::string message;
};

/**
 * @struct AtcMessageReceivedEvent
 * @brief Event fired when an ATC message is received
 */
struct AtcMessageReceivedEvent {
    std::string sentFrom;
    std::string message;
};

struct ClientTextMessageEvent {
    std::string sentFrom;
    std::string message;
    std::optional<std::array<int, 3>> colour;
    bool isUrgent = false;
    bool ownChannel = false;
};

/**
 * @interface TextMessageAPI
 * @brief Interface for TextMessage operations
 */
class TextMessageAPI {
public:
    virtual ~TextMessageAPI() = default;

    /**
     * @brief Send a text message to the server channel
     * @return Connection state
     */
    virtual void SendClientMessage(const ClientTextMessageEvent message) = 0;

};

} // namespace PluginSDK::TextMessage
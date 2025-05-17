#pragma once
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace PluginSDK::Chat {

enum class ParameterType { String, Number, Boolean };

struct CommandParameter {
    std::string name;
    ParameterType type;
    bool typeExplicit = false;
    bool required = false;
    int length = 0;
    int minLength = 0;
    int maxLength = 0;
};

// Command definition
struct CommandDefinition {
    std::string name;
    std::string description;
    std::vector<CommandParameter> parameters;
    bool lastParameterHasSpaces = false;
};

// Command validation result
struct ValidationResult {
    bool isValid;
    std::optional<std::string> message;
};

struct CommandResult {
    bool success;
    std::optional<std::string> message;
};

// Command Provider interface
class CommandProvider {
public:
    virtual ~CommandProvider() = default;

    // Execute a command with arguments
    virtual CommandResult Execute(
        const std::string& commandId, const std::vector<std::string>& args)
        = 0;

    // Validate command parameters
    virtual ValidationResult ValidateParameters(
        const std::string& commandId, const std::vector<std::string>& args, bool strict)
        = 0;
};

// Registration token
class RegistrationToken {
public:
    virtual ~RegistrationToken() = default;

    // Non-copyable but movable
    RegistrationToken(const RegistrationToken&) = delete;
    RegistrationToken& operator=(const RegistrationToken&) = delete;
    RegistrationToken(RegistrationToken&&) = default;
    RegistrationToken& operator=(RegistrationToken&&) = default;

protected:
    RegistrationToken() = default;
};

// Chat API
class ChatAPI {
public:
    virtual ~ChatAPI() = default;

   /**
     * @brief Registers a command with the system
     *
     * @param name Command name without dot prefix (e.g., "speed" for ".speed")
     * @param definition Command definition
     * @param provider Provider implementation handling execution and validation
     * @return Command ID if successful, empty string if failed (e.g., command name
     * collision or reserved name)
     *
     * @note The following command names are reserved and cannot be registered:
     *       "hello", "wx", "clear", "dev", "rings", "vis", "wallop", "chat", "center"
     */
    virtual std::string registerCommand(const std::string& name,
        const CommandDefinition& definition, std::shared_ptr<CommandProvider> provider)
        = 0;

    virtual bool unregisterCommand(const std::string& commandId) = 0;

    virtual void sendSystemMessage(
        const std::string& from, const std::string& message, bool isError)
        = 0;

    virtual void sendExternalMessage(
        const std::string& from, const std::string& message, bool isError)
        = 0;
};

} // namespace PluginSDK::Chat
// plugin/sdk/Sectors.h
#pragma once
#include <string>

namespace PluginSDK::Sectors {

/**
 * @struct OwnedSectorUpdatedEvent
 * @brief Event fired when owned sector information is updated
 */
struct OwnedSectorUpdatedEvent {
    int featureCount; // Number of geographic features in the updated sector
    bool hasInflatedBoundary;
};

} // namespace PluginSDK::Sectors
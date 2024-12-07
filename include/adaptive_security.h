#ifndef ADAPTIVE_SECURITY_H
#define ADAPTIVE_SECURITY_H

#include <vector>
#include <cstdint>
#include "skein3.h"
#include "security_monitor.h"

class AdaptiveSecurity {
public:
    enum class ThreatLevel {
        LOW,
        MEDIUM,
        HIGH,
        CRITICAL
    };
    
    static void adjustSecurityParameters(
        Skein3::Config& config,
        ThreatLevel threat_level
    );
    
    static ThreatLevel analyzeThreatLevel(
        const std::vector<uint8_t>& data,
        const SecurityMonitor::SecurityMetrics& metrics
    );
};

#endif // ADAPTIVE_SECURITY_H 
#ifndef SECURITY_MONITOR_H
#define SECURITY_MONITOR_H

#include <vector>
#include <cstdint>
#include <string>
#include <chrono>

struct ThreatIndicator {
    enum class Type {
        PATTERN_ANOMALY,
        ENTROPY_DROP,
        TIMING_ANOMALY,
        COLLISION_ATTEMPT,
        STRUCTURE_VIOLATION
    };

    Type type;
    float severity;
    std::string description;
    uint64_t timestamp;
};

struct SecurityMetricsImpl {
    float entropy_level;
    float pattern_complexity;
    float attack_probability;
    std::vector<ThreatIndicator> threats;
};

class SecurityMonitor {
public:
    struct SecurityMetrics {
        float entropy_level;
        float pattern_complexity;
        float attack_probability;
        std::vector<ThreatIndicator> threats;
    };
    
    static SecurityMetrics analyzeHashOperation(
        const std::vector<uint8_t>& input,
        const std::vector<uint8_t>& output
    );

protected:
    static void detectThreats(
        SecurityMetricsImpl& metrics,
        const std::vector<uint8_t>& input,
        const std::vector<uint8_t>& output
    );
    
    static float calculateAttackProbability(
        const SecurityMetricsImpl& metrics
    );
};

#endif // SECURITY_MONITOR_H
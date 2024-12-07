#ifndef SECURITY_MONITOR_H
#define SECURITY_MONITOR_H

#include <vector>
#include <cstdint>

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
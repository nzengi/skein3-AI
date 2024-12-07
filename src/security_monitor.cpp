#include "security_monitor.h"
#include "skein3.h"
#include <cmath>
#include <numeric>
#include <map>
#include <deque>

// Threat indicator implementation
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

class SecurityMetricsImpl {
public:
    float entropy_level;
    float pattern_complexity;
    float attack_probability;
    std::vector<ThreatIndicator> threats;
    
    // Historical data for trend analysis
    std::deque<float> entropy_history;
    std::deque<float> complexity_history;
    static constexpr size_t HISTORY_SIZE = 1000;
    
    // Pattern detection
    std::map<std::vector<uint8_t>, size_t> pattern_frequency;
    static constexpr size_t PATTERN_SIZE = 8;
};

SecurityMonitor::SecurityMetrics SecurityMonitor::analyzeHashOperation(
    const std::vector<uint8_t>& input,
    const std::vector<uint8_t>& output
) {
    SecurityMetricsImpl metrics;
    
    // Calculate input entropy
    std::array<size_t, 256> histogram{};
    for (uint8_t byte : input) {
        histogram[byte]++;
    }
    
    float entropy = 0.0f;
    for (size_t count : histogram) {
        if (count > 0) {
            float p = static_cast<float>(count) / input.size();
            entropy -= p * std::log2(p);
        }
    }
    metrics.entropy_level = entropy / 8.0f; // Normalize to [0,1]
    
    // Update entropy history
    metrics.entropy_history.push_back(metrics.entropy_level);
    if (metrics.entropy_history.size() > SecurityMetricsImpl::HISTORY_SIZE) {
        metrics.entropy_history.pop_front();
    }
    
    // Analyze patterns
    for (size_t i = 0; i <= input.size() - SecurityMetricsImpl::PATTERN_SIZE; ++i) {
        std::vector<uint8_t> pattern(
            input.begin() + i,
            input.begin() + i + SecurityMetricsImpl::PATTERN_SIZE
        );
        metrics.pattern_frequency[pattern]++;
    }
    
    // Calculate pattern complexity
    float max_freq = 0;
    for (const auto& [pattern, freq] : metrics.pattern_frequency) {
        max_freq = std::max(max_freq, static_cast<float>(freq));
    }
    metrics.pattern_complexity = 1.0f - (max_freq / (input.size() - SecurityMetricsImpl::PATTERN_SIZE + 1));
    
    // Update complexity history
    metrics.complexity_history.push_back(metrics.pattern_complexity);
    if (metrics.complexity_history.size() > SecurityMetricsImpl::HISTORY_SIZE) {
        metrics.complexity_history.pop_front();
    }
    
    // Detect threats
    detectThreats(metrics, input, output);
    
    // Calculate attack probability
    metrics.attack_probability = calculateAttackProbability(metrics);
    
    // Convert internal metrics to public interface
    SecurityMonitor::SecurityMetrics result;
    result.entropy_level = metrics.entropy_level;
    result.pattern_complexity = metrics.pattern_complexity;
    result.attack_probability = metrics.attack_probability;
    result.threats = std::move(metrics.threats);
    
    return result;
}

void SecurityMonitor::detectThreats(
    SecurityMetricsImpl& metrics,
    const std::vector<uint8_t>& input,
    const std::vector<uint8_t>& output
) {
    // Entropy anomaly detection
    if (!metrics.entropy_history.empty()) {
        float avg_entropy = std::accumulate(
            metrics.entropy_history.begin(),
            metrics.entropy_history.end(),
            0.0f
        ) / metrics.entropy_history.size();
        
        if (metrics.entropy_level < avg_entropy * 0.8f) {
            ThreatIndicator threat;
            threat.type = ThreatIndicator::Type::ENTROPY_DROP;
            threat.severity = (avg_entropy - metrics.entropy_level) / avg_entropy;
            threat.description = "Significant drop in entropy detected";
            threat.timestamp = std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::now()
            );
            metrics.threats.push_back(threat);
        }
    }
    
    // Pattern anomaly detection
    if (!metrics.complexity_history.empty()) {
        float avg_complexity = std::accumulate(
            metrics.complexity_history.begin(),
            metrics.complexity_history.end(),
            0.0f
        ) / metrics.complexity_history.size();
        
        if (metrics.pattern_complexity < avg_complexity * 0.7f) {
            ThreatIndicator threat;
            threat.type = ThreatIndicator::Type::PATTERN_ANOMALY;
            threat.severity = (avg_complexity - metrics.pattern_complexity) / avg_complexity;
            threat.description = "Suspicious input pattern detected";
            threat.timestamp = std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::now()
            );
            metrics.threats.push_back(threat);
        }
    }
    
    // Output structure verification
    if (output.size() != 64) { // Expected hash size
        ThreatIndicator threat;
        threat.type = ThreatIndicator::Type::STRUCTURE_VIOLATION;
        threat.severity = 1.0f;
        threat.description = "Invalid hash output size";
        threat.timestamp = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        );
        metrics.threats.push_back(threat);
    }
}

float SecurityMonitor::calculateAttackProbability(const SecurityMetricsImpl& metrics) {
    float probability = 0.0f;
    
    // Weight different factors
    const float ENTROPY_WEIGHT = 0.3f;
    const float COMPLEXITY_WEIGHT = 0.3f;
    const float THREAT_WEIGHT = 0.4f;
    
    // Entropy factor
    float entropy_factor = 1.0f - metrics.entropy_level;
    
    // Complexity factor
    float complexity_factor = 1.0f - metrics.pattern_complexity;
    
    // Threat factor
    float threat_factor = 0.0f;
    if (!metrics.threats.empty()) {
        float max_severity = 0.0f;
        for (const auto& threat : metrics.threats) {
            max_severity = std::max(max_severity, threat.severity);
        }
        threat_factor = max_severity;
    }
    
    // Combine factors
    probability = ENTROPY_WEIGHT * entropy_factor +
                 COMPLEXITY_WEIGHT * complexity_factor +
                 THREAT_WEIGHT * threat_factor;
    
    return std::min(1.0f, std::max(0.0f, probability));
} 
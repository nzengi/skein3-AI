#include "security_monitor.h"
#include "skein3.h"
#include <cmath>
#include <numeric>
#include <map>
#include <deque>

namespace {
    std::deque<float> entropy_history;
    std::deque<float> complexity_history;
    std::map<std::vector<uint8_t>, size_t> pattern_frequency;
    
    static constexpr size_t HISTORY_SIZE = 1000;
    static constexpr size_t PATTERN_SIZE = 8;
}

SecurityMonitor::SecurityMetrics SecurityMonitor::analyzeHashOperation(
    const std::vector<uint8_t>& input,
    const std::vector<uint8_t>& output
) {
    SecurityMetricsImpl metrics;
    
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
    metrics.entropy_level = entropy / 8.0f;
    
    entropy_history.push_back(metrics.entropy_level);
    if (entropy_history.size() > HISTORY_SIZE) {
        entropy_history.pop_front();
    }
    
    pattern_frequency.clear();
    for (size_t i = 0; i <= input.size() - PATTERN_SIZE; ++i) {
        std::vector<uint8_t> pattern(
            input.begin() + i,
            input.begin() + i + PATTERN_SIZE
        );
        pattern_frequency[pattern]++;
    }
    
    float max_freq = 0;
    for (const auto& [pattern, freq] : pattern_frequency) {
        max_freq = std::max(max_freq, static_cast<float>(freq));
    }
    metrics.pattern_complexity = 1.0f - (max_freq / (input.size() - PATTERN_SIZE + 1));
    
    complexity_history.push_back(metrics.pattern_complexity);
    if (complexity_history.size() > HISTORY_SIZE) {
        complexity_history.pop_front();
    }
    
    detectThreats(metrics, input, output);
    
    metrics.attack_probability = calculateAttackProbability(metrics);
    
    SecurityMetrics result;
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
    if (!entropy_history.empty()) {
        float avg_entropy = std::accumulate(
            entropy_history.begin(),
            entropy_history.end(),
            0.0f
        ) / entropy_history.size();
        
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
    
    if (!complexity_history.empty()) {
        float avg_complexity = std::accumulate(
            complexity_history.begin(),
            complexity_history.end(),
            0.0f
        ) / complexity_history.size();
        
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
}

float SecurityMonitor::calculateAttackProbability(const SecurityMetricsImpl& metrics) {
    float probability = 0.0f;
    
    const float ENTROPY_WEIGHT = 0.3f;
    const float COMPLEXITY_WEIGHT = 0.3f;
    const float THREAT_WEIGHT = 0.4f;
    
    float entropy_factor = 1.0f - metrics.entropy_level;
    
    float complexity_factor = 1.0f - metrics.pattern_complexity;
    
    float threat_factor = 0.0f;
    if (!metrics.threats.empty()) {
        float max_severity = 0.0f;
        for (const auto& threat : metrics.threats) {
            max_severity = std::max(max_severity, threat.severity);
        }
        threat_factor = max_severity;
    }
    
    probability = ENTROPY_WEIGHT * entropy_factor +
                 COMPLEXITY_WEIGHT * complexity_factor +
                 THREAT_WEIGHT * threat_factor;
    
    return std::min(1.0f, std::max(0.0f, probability));
} 
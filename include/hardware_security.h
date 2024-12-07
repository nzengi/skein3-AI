#ifndef HARDWARE_SECURITY_H
#define HARDWARE_SECURITY_H

#include <vector>
#include <cstdint>

class HardwareSecurityModule {
public:
    virtual ~HardwareSecurityModule() = default;
    virtual bool initializeSecureEnclave() = 0;
    virtual std::vector<uint8_t> generateSecureHash(const std::vector<uint8_t>& data) = 0;
    virtual bool verifyHashInEnclave(const std::vector<uint8_t>& hash) = 0;
};

#endif // HARDWARE_SECURITY_H 
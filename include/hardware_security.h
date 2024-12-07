class HardwareSecurityModule {
public:
    // TPM/SGX Integration
    bool initializeSecureEnclave();
    std::vector<uint8_t> generateSecureHash(const std::vector<uint8_t>& data);
    bool verifyHashInEnclave(const std::vector<uint8_t>& hash);
}; 
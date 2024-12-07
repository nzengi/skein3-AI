#include "skein3.h"
#include <iostream>
#include <iomanip>  // setw ve setfill için
#include <cassert>

void testBasicFunctionality() {
    try {
        // Test verisi hazırla
        std::string test_message = "Merhaba Skein3!";
        std::vector<uint8_t> data(test_message.begin(), test_message.end());

        // Temel hash işlemi
        Skein3::Config config;
        config.size = Skein3::HashSize::HASH_512;
        auto hash = Skein3::hash(data, config);

        std::cout << "Hash uzunluğu: " << hash.size() << " bytes\n";
        std::cout << "Hash değeri: ";
        for (uint8_t byte : hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(byte);
        }
        std::cout << std::endl;

        // Hash doğrulama - verifyHash fonksiyonunu doğrudan karşılaştırma ile değiştiriyoruz
        auto verify_hash = Skein3::hash(data, config);
        assert(verify_hash == hash);
        std::cout << "Hash doğrulama başarılı!\n";

    } catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << std::endl;
    }
}

// Main fonksiyonu ekliyoruz
int main() {
    testBasicFunctionality();
    return 0;
} 
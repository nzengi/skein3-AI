#include "skein3.h"
#include <cassert>
#include <iostream>

void test_skein3() {
    // Test mesajları
    std::string mesaj1 = "Test mesajı 1";
    std::string mesaj2 = "Test mesajı 2";

    // Hash hesapla
    auto hash1 = skein3_hash(std::vector<uint8_t>(mesaj1.begin(), mesaj1.end()), 64);
    auto hash2 = skein3_hash(std::vector<uint8_t>(mesaj2.begin(), mesaj2.end()), 64);

    // Hashlerin farklı olduğunu kontrol et
    assert(hash1 != hash2);

    // Hash uzunluğunu kontrol et
    assert(hash1.size() == 64);
    assert(hash2.size() == 64);

    // Boş mesaj testi
    auto empty_hash = skein3_hash(std::vector<uint8_t>(), 64);
    assert(empty_hash.size() == 64);
    
    // Uzun mesaj testi
    std::vector<uint8_t> long_message(1000000, 'a');
    auto long_hash = skein3_hash(long_message, 64);
    assert(long_hash.size() == 64);
    
    // Farklı hash boyutları için testler
    auto hash_32 = skein3_hash(std::vector<uint8_t>{1,2,3}, 32);
    assert(hash_32.size() == 32);

    std::cout << "Tüm testler başarıyla geçti!" << std::endl;
}

int main() {
    test_skein3();
    return 0;
}

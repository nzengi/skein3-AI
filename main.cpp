#include "skein3.h"
#include <iostream>
#include <iomanip>

int main() {
    // Örnek mesaj
    std::string mesaj = "Merhaba, bu Skein3 hash fonksiyonunun C++ uygulamasıdır.";
    std::vector<uint8_t> mesaj_bytes(mesaj.begin(), mesaj.end());

    // Hash hesapla
    auto hash = skein3_hash(mesaj_bytes, 64);

    // Hash'i yazdır
    std::cout << "Skein3 Hash: ";
    for (uint8_t byte : hash) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    std::cout << std::endl;

    return 0;
}

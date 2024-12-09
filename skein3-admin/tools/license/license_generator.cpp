#include "../include/license.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <user_id>\n";
        return 1;
    }

    std::string user_id = argv[1];
    if (user_id.length() != 7) {
        std::cout << "User ID must be 7 characters\n";
        return 1;
    }

    std::string validation = LicenseManager::generateValidationCode(user_id);
    std::cout << "License key: SKEIN3-" << user_id << "-" << validation << "\n";
    return 0;
} 
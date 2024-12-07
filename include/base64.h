#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <vector>

class Base64 {
public:
    static std::string encode(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> decode(const std::string& encoded);
    
private:
    static const std::string base64_chars;
    static inline bool is_base64(unsigned char c);
};

#endif // BASE64_H
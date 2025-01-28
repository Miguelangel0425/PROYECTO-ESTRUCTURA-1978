#pragma once
#include <string>

class CesarCipher {
public:
    static std::string encrypt(const std::string& text, int shift = 3);
    static std::string decrypt(const std::string& text, int shift = 3);
};

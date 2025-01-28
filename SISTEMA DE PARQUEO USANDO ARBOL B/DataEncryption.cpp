#include "DataEncryption.h"

std::string CesarCipher::encrypt(const std::string& text, int shift) {
    std::string result = text;
    for(char& c : result) {
        if(isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = base + (c - base + shift) % 26;
        }
    }
    return result;
}

std::string CesarCipher::decrypt(const std::string& text, int shift) {
    return encrypt(text, 26 - shift);
}

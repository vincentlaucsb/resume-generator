#include "text.h"

namespace resume {
    bool is_capital(char ch) {
        return ch >= 'A' && ch <= 'Z';
    }

    std::string lower(std::string_view str) {
        std::string ret;
        for (char ch : str) {
            ret += tolower(ch);
        }

        return ret;
    }
}
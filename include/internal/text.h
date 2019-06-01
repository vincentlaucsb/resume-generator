#pragma once
// Contains text processing functions
namespace resume {
    inline bool is_capital(char ch) {
        return ch >= 'A' && ch <= 'Z';
    }

    // Turn "CamelCase" into "Camel Case"
    inline std::string split_camel_case(const std::string& str) {
        std::string new_str;

        for (size_t i = 0; i < str.size(); i++) {
            const char & ch = str[i];
            if (is_capital(ch)) {
                if (i == 0 || !is_capital(str[i - 1])) {
                    new_str += " ";
                }
            }

            new_str += ch;
        }

        return new_str;
    }
}
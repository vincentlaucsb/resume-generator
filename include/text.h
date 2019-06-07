#pragma once
#include <string>
#include <string_view>

// Contains text processing functions
namespace resume {
    bool is_capital(char ch);

    // Return lowercase version of a string
    std::string lower(std::string_view);

    // Turn "CamelCase" into "Camel Case"
    inline std::string split_camel_case(const std::string& str) {
        std::string new_str;

        for (size_t i = 0; i < str.size(); i++) {
            const char & ch = str[i];
            
            // Consecutive sequences of capital letters do not get split
            if (is_capital(ch)) {
                if (i == 0 || !is_capital(str[i - 1])) {
                    new_str += " ";
                }
            }

            new_str += ch;
        }

        return new_str;
    }

    std::string& replace(std::string& str, std::string_view substr, std::string_view replace);

    // Converts -- to &mdash;
    void dashify(std::string& str);

    // Wraps URLs with <a> tags
    void url(std::string& str);
}
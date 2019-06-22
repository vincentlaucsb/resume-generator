#pragma once
#include <array>
#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>

// Contains text processing functions
namespace resume {
    constexpr std::array<bool, 256> make_whitespace_array() {
        std::array<bool, 256> arr = {};
        for (int i = -128; i < 128; i++) {
            char ch = (char)i;
            if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
                arr[i + 128] = true;
            }
            else {
                arr[i + 128] = false;
            }
        }

        return arr;
    }

    constexpr std::array<bool, 256> WHITESPACE_CHARS = make_whitespace_array();

    bool is_capital(char ch);

    // Split a string by a delimiter
    template<char Delim>
    std::vector<std::string> split(std::string_view);

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

    template<char Delim>
    std::vector<std::string> split(std::string_view in)
    {
        std::vector<std::string> ret = {};
        size_t beg = 0;
        for (size_t i = 0; i < in.size(); i++) {
            const char & ch = in[i];
            if (ch == Delim) {
                if (beg != i) {
                    ret.push_back(std::string(in.substr(beg, i - beg)));
                    beg = i + 1;
                }
            }
            else if (i == beg && WHITESPACE_CHARS[ch + 128]) {
                // Allow trailing spaces after delimiters
                beg = i + 1;
            }
        }

        ret.push_back(std::string(in.substr(beg)));
        return ret;
    }

    // Formats a string with the given arguments
    std::string format(std::string_view, const std::unordered_map<std::string, std::string>&);
}
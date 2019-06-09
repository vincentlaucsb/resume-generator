#include "text.h"
#include <regex>
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

    std::string& replace(std::string& str, std::string_view substr, std::string_view replace) {
        while (str.find(substr) != str.npos) {
            str.replace(str.find(substr), substr.size(), replace);
        }

        return str;
    }

    // Converts -- to &mdash;
    void dashify(std::string& str) {
        replace(str, "--", "&ndash;");
    }

    void url(std::string& str) {
        std::regex url_re("((?:(?:https*:\/\/)|(?:www))[A-Za-z1-9.?/=&-]*)");
        str = std::regex_replace(str, url_re, "<a href=\"$&\">$&</a>");
    }

    std::string format(std::string_view str, const std::unordered_map<std::string, std::string>& args)
    {
        bool in_key = false;
        std::string key = "", ret = "";

        for (size_t i = 0; i < str.size(); i++) {
            const char & ch = str[i];
            if (ch == '{') {
                if (!in_key) {
                    in_key = true;
                }
                else {
                    throw std::runtime_error("Unexpected left bracket");
                }
            }
            else if (ch == '}') {
                if (in_key) {
                    in_key = false;
                }
                else {
                    throw std::runtime_error("Unexpected right bracket");
                }

                // Replace key
                auto value = args.find(key);
                if (value == args.end()) {
                    throw std::runtime_error(std::string("Key ") + key + " was not found in args.");
                }

                key.clear();
                ret += value->second;
            }
            else {
                if (in_key) {
                    key += ch;
                }
                else {
                    ret += ch;
                }
            }
        }

        return ret;
    }
}
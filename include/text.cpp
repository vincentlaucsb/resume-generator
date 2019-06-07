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
}
#include "StringUtils.h"
#include <cctype>
#include <sstream>

namespace nc 
{
    std::string StringUtils::ToUpper(const std::string & str) 
    {
        std::string result = str;
        // Takes each character in a string and makes them the uppercase variant.
        for (char& c : result) {
            c = std::toupper(c);
        }
        return result;
    }

    std::string StringUtils::ToLower(const std::string & str) 
    {
        std::string result = str;
        // Takes each character in a string and makes them the lowercase variant.
        for (char& c : result) {
            c = std::tolower(c);
        }
        return result;
    }

    bool StringUtils::IsEqualIgnoreCase(const std::string & str1, const std::string & str2) 
    {
        // Checks to see if length matches. If not, returns false.
        if (str1.length() != str2.length()) {
            return false;
        }
        //Checks if the lowercase versions of both strings match. If not, returns false.
        for (size_t i = 0; i < str1.length(); ++i) {
            if (std::tolower(str1[i]) != std::tolower(str2[i])) {
                return false;
            }
        }
        return true;
    }

    std::string StringUtils::CreateUnique(const std::string & str)
    {
        static uint32_t unique = 0;
        // Prints out a string that changes each time the function is called using an incrementing static variable.
        return str + std::to_string(unique++);
    }
}
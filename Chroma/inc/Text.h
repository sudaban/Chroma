#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>
#include <cctype>

class Text 
{
public:
    static std::vector<std::string> explode(const std::string& delimiter, const std::string& str) 
    {
        std::vector<std::string> result;
        size_t start = 0, end;
        while ((end = str.find(delimiter, start)) != std::string::npos) {
            result.emplace_back(str.substr(start, end - start));
            start = end + delimiter.length();
        }
        result.emplace_back(str.substr(start));
        return result;
    }

    static void replace(const std::string& from, const std::string& to, std::string& text) 
    {
        if (from.empty()) return;
        size_t pos = 0;
        while ((pos = text.find(from, pos)) != std::string::npos)
        {
            text.replace(pos, from.length(), to);
            pos += to.length();
        }
    }

    static void erase(const std::string& target, std::string& text)
    {
        if (target.empty()) return;
        size_t pos = 0;
        while ((pos = text.find(target, pos)) != std::string::npos) 
        {
            text.erase(pos, target.length());
        }
    }

    static std::string to_erase(const std::string& target, std::string text)
    {
        if (target.empty()) return text;
        size_t pos = 0;
        while ((pos = text.find(target, pos)) != std::string::npos)
        {
            text.erase(pos, target.length());
        }
        return text;
    }

    static bool isDigit(const std::string& text) 
    {
        if (text.empty()) return false;
        for (char c : text)
            if (!std::isdigit(static_cast<unsigned char>(c)))
                return false;
        return true;
    }

    static bool isAscii(const std::string& text) 
    {
        if (text.empty()) return false;
        for (char c : text)
            if (static_cast<unsigned char>(c) < 32 || static_cast<unsigned char>(c) > 126)
                return false;
        return true;
    }

    static std::string upper(std::string text)
    {
        for (char& c : text)
            c = std::toupper(static_cast<unsigned char>(c));
        return text;
    }

    static std::string lower(std::string text)
    {
        for (char& c : text)
            c = std::tolower(static_cast<unsigned char>(c));
        return text;
    }

    static std::string parser(const std::string& key, const std::string& text)
    {
        const std::string key_prefix = key + "|";
        const size_t start_pos = text.find(key_prefix);
        if (start_pos == std::string::npos) return "";

        const size_t value_start = start_pos + key_prefix.length();
        const size_t end_pos = text.find('\n', value_start);

        return (end_pos == std::string::npos)
            ? text.substr(value_start)
            : text.substr(value_start, end_pos - value_start);
    }
};

#endif // TEXT_H

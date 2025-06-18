#include "Log.h"

#ifdef _WIN32
void setConsoleColor(LogType type)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD color;

    switch (type) {
    case LogType::Error:   color = 12; break; // Red
    case LogType::Warning: color = 14; break; // Yellow
    case LogType::Info:    color = 11; break; // Cyan
    case LogType::Debug:   color = 13; break; // Magenta
    case LogType::Success: color = 10; break; // Green
    default:               color = 7;  break; // White
    }

    SetConsoleTextAttribute(hConsole, color);
}
#else
std::string getAnsiColorCode(LogType type) {
    switch (type) {
    case LogType::Error:   return "\033[31m"; // Red
    case LogType::Warning: return "\033[33m"; // Yellow
    case LogType::Info:    return "\033[36m"; // Cyan
    case LogType::Debug:   return "\033[35m"; // Magenta
    case LogType::Success: return "\033[32m"; // Green
    default:               return "\033[0m";  // Reset
    }
}
#endif

std::string getLogTitle(LogType type) {
    switch (type) {
    case LogType::Error:   return "[ERROR] ";
    case LogType::Warning: return "[WARNING] ";
    case LogType::Info:    return "[INFO] ";
    case LogType::Debug:   return "[DEBUG] ";
    case LogType::Success: return "[SUCCESS] ";
    default:               return "[LOG] ";
    }
}

void Logger(const std::string& message, LogType type) {
    std::string title = getLogTitle(type);

#ifdef _WIN32
    setConsoleColor(type);
    std::cout << title << message << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset to default
#else
    std::cout << getAnsiColorCode(type) << title << message << "\033[0m" << std::endl;
#endif
}
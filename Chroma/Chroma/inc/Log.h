#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

enum class LogType
{
    Error,
    Warning,
    Info,
    Debug,
    Success
};

#ifdef _WIN32
void setConsoleColor(LogType type);
#else
std::string getAnsiColorCode(LogType type);
#endif

std::string getLogTitle(LogType type);

void Logger(const std::string& message, LogType type);

#endif

#include <cstdio>

#include "Logger.h"

/* delete this line to turn off debug messages */
#define DEBUG_MODE

void
Logger::ErrorDie(const std::string& message)
{
    fprintf(stderr, "[ERROR] %s\n", message.c_str());
    exit(1);
}

void
Logger::Network(const std::string& message)
{
    printf("[NETWORK] %s\n", message.c_str());
}

void
Logger::Warning(const std::string& message)
{
    printf("[WARN] %s\n", message.c_str());
}

void
Logger::Debug(const std::string& message)
{
    #ifdef DEBUG_MODE
    printf("[DEBUG] %s\n", message.c_str());
    #endif
}

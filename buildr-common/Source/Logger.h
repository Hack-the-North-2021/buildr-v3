#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

namespace Logger
{
    void ErrorDie(const std::string& message);
    void Network(const std::string& message);
    void Warning(const std::string& message);
    void Debug(const std::string& message);
}

#endif // __LOGGER_H__

#ifndef LOGGER_H
#define LOGGER_H
#include <string>

class Logger
{
public:
    static void error(std::string s);
    static void warning(std::string s);
    static void info(std::string s);
    static void verbose(std::string s);
    static void setMode(int mode);
private:
    static int mode;
};

#endif // LOGGER_H

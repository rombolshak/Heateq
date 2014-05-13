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
    static void debug(std::string s);
    static void setMode(int mode);
    static void write(std::string s);
private:
    static int _mode;
};

#endif // LOGGER_H

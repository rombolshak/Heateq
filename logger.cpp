#include "logger.h"
#include <iostream>
#include <time.h>
#include <string>
#include <stdio.h>

int Logger::_mode = 3;

void Logger::write(std::string s)
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%X", &tstruct);
    std::cout << buf << ": " << s;
}

void Logger::error(std::string s)
{
    if (_mode > 0) {
	std::cout << "!!! [ERROR]: " << s << std::endl;
    }
}

void Logger::warning(std::string s)
{
    if (_mode > 1) {
	std::cout << "! [WARNING]: " << s << std::endl;
    }
}

void Logger::info(std::string s)
{
    if (_mode > 2) {
	std::cout << "[INFO]: " << s << std::endl;
    }
}

void Logger::verbose(std::string s)
{
    if (_mode > 3) {
	std::cout << "[VERBOSE]: " << s << std::endl;
    }
}

void Logger::setMode(int mode)
{
    if (mode >= 0 && mode <= 4) {
	Logger::_mode = mode;
    }
 
    else {
	error("Unrecognized logger level, fallback to default: info"); 
	Logger::_mode = 3;
    }    
}
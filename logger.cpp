#include "logger.h"
#include <iostream>

static int Logger::mode = '2';

void Logger::error(std::string s)
{
    if (mode >= 0) {
	std::cout << "!!! [ERROR]: " << s << std::endl;
    }
}

void Logger::warning(std::string s)
{
    if (mode >= 1) {
	std::cout << "! [WARNING]: " << s << std::endl;
    }
}

void Logger::info(std::string s)
{
    if (mode >= 2) {
	std::cout << "[INFO]: " << s << std::endl;
    }
}

void Logger::verbose(std::string s)
{
    if (mode >= 3) {
	std::cout << "[VERBOSE]: " << s << std::endl;
    }
}

void Logger::setMode(char mode)
{
    switch (mode) {
	case 'e': Logger::mode = 0; break;
	case 'w': Logger::mode = 1; break;
	case 'i': Logger::mode = 2; break;
	case 'v': Logger::mode = 3; break;
	default: 
	    error("Unrecognized logger level, fallback to default: info"); 
	    Logger::mode = 2;
    }
}

#include "logger.h"
#include <iostream>
#include <time.h>
#include <string>
#include <stdio.h>
#include "mpi.h"

int Logger::_mode = 3;

void Logger::write(std::string s)
{
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%X", &tstruct);
    if (0 == rank) {
        std::cout << buf << ": " << s << std::endl;
    }
}

void Logger::error(std::string s)
{
    if (_mode > 0) {
        write("!!! [ERROR]: " + s);
    }
}

void Logger::warning(std::string s)
{
    if (_mode > 1) {
        write("! [WARNING]: " + s);
    }
}

void Logger::info(std::string s)
{
    if (_mode > 2) {
        write("[INFO]: " + s);
    }
}

void Logger::verbose(std::string s)
{
    if (_mode > 3) {
        write("[VERBOSE]: " + s);
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

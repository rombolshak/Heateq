#include <iostream>
#include <cmath>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>
#include "task.h"
#include "explicitheatsolver.h"
#include "implicitheatsolver.h"
#include "plotdatapreparer.h"
#include "logger.h"

/*
 * In order to solve another heat equation
 * you need to change 4 functions below
 * and recompile the program.
 * 
 * Create a math expression parser is huge overkill imho
 */

const double alpha = 1;

double f(double x, double t) {
    return 0;
}

double initial(double x) {
    return std::sin(x) * std::sin(x) + std::cos(x);
}

double boundaryLeft(double t) {
    return 0;
}

double boundaryRight(double t) {
    return 0;
}

void printUsage(const char *name) {
    std::cout << 
    "Usage: " << name << " [options] output" << std::endl <<
    "Options: " << std::endl <<
    "-h|--help\t\tShow this usage message and quit" << std::endl <<
    "-m n|--time=n\t\tMaximum time to calculate; default = 10" << std::endl <<
    "-l n|--left=n\t\tLeft coordinate; default = 0" << std::endl << 
    "-r n|--right=n\t\tRight coordinate; default = 1" << std::endl <<
    "-t n|--timestep=n\tTime step; default = 0.001" << std::endl <<
    "-c n|--coordstep=n\tCoordinate step; default = 0.05. Please notice that time step depends on coordinate step in such way: ts <= 0.5 * cs^2" << std::endl <<
    "-i n|--times=n\t\tTimes count; if given, time step option will be ignored" << std::endl <<
    "-o n|--coords=n\t\tCoordinates count; if given, coordinate step option will be ognored" << std::endl <<
    "-v n|--verbose=n\t\tVerbose level: 0 -- errors only, 1 -- 0 + warnings, 2 -- 1 + info, 3 -- full verbose. Default: info" << 
    std::endl <<
    "output\t\t\tName without format. Program'll generate 'output.dat' with plot data and 'output.pl' with script that will produce 'output.gif' result plot" << std::endl <<
    std::endl;
}

int main(int argc, char **argv) {
    int param = 0;
    int option_index = -1;
    opterr = 0;
    
    char ** itr = std::find(argv, argv+argc, std::string("-v"));
    if (itr != argv+argc && ++itr != argv+argc) {
	Logger::setMode(atoi(*itr));
    }
    
    const char *shortOptions = "hm:l:r:t:c:i:o:v:";
    const struct option longOptions[] = {
	{"help",no_argument,NULL,'h'},
	{"time",required_argument,NULL,'m'},
	{"left",required_argument,NULL,'l'},
	{"right",required_argument,NULL,'r'},
	{"timestep",required_argument,NULL,'t'},
	{"coordstep",required_argument,NULL,'c'},
	{"times",required_argument,NULL,'i'},
	{"coords",required_argument,NULL,'o'},
	{"verbose",required_argument,NULL,'v'},
	{NULL,0,NULL,0}
    };
    
    double left=0, right=1, time=10, timestep=0.001, coordstep=0.05, times=0, coords=0;
    
    while ((param = getopt_long_only(argc, argv, shortOptions, longOptions, &option_index)) != -1) {
	switch (param) {
	    case '?': std::cout << "Unknown option! Read this manual carefully" << std::endl; printUsage(argv[0]); return 0;
	    case 'h': printUsage(argv[0]); return 0;
	    case 'm': time = atof(optarg); break;
	    case 'l': left = atof(optarg); break;
	    case 'r': right = atof(optarg); break;
	    case 't': timestep = atof(optarg); break;
	    case 'c': coordstep = atof(optarg); break;
	    case 'i': times = atof(optarg); break;
	    case 'o': coords = atof(optarg); break;
	}
    }
    
    
    Logger::verbose("check verbose");
    Task *task = new Task(alpha, &f, &initial, &boundaryLeft, &boundaryRight, time, left, right, timestep, coordstep, coords, times);
    PlotDataPreparer writer;
    
    writer.WriteData(ExplicitHeatSolver::solve(task), "exp");
    //preparer.WriteData(ImplicitHeatSolver::solve(task), "imp");
    return 0;
}

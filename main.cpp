#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include "task.h"
#include "solver.h"
#include "timeindependentsolver.h"
#include "plotdatapreparer.h"
#include "logger.h"
#include "mpi.h"

extern double f(double x, double t);
extern std::complex< double > initial(double x);
extern std::complex< double > boundaryLeft(double t);
extern std::complex< double > boundaryRight(double t);

void printUsage(const char *name) {
    std::cout << 
    "Usage: " << name << " [options]" << std::endl <<
    "Options: " << std::endl <<
    "-h|--help\t\tShow this usage message and quit" << std::endl <<
    "-i|--independent\tSolve time-independent problem" << std::endl <<
    "-m n|--time=n\t\tMaximum time to calculate; default = 1, or 0 if time-independent" << std::endl <<
    "-l n|--left=n\t\tLeft coordinate; default = 0" << std::endl << 
    "-r n|--right=n\t\tRight coordinate; default = 1" << std::endl <<
    "-t n|--timestep=n\tTime step; default = 0.001, or ignored if time-independent" << std::endl <<
    "-c n|--coordstep=n\tCoordinate step; default = 0.05. Please notice that time step depends on coordinate step in such way: ts <= 0.5 * cs^2" << std::endl <<
    "--times=n\t\tTimes count; if given, time step option will be ignored" << std::endl <<
    "--coords=n\t\tCoordinates count; if given, coordinate step option will be ognored" << std::endl <<
    "-o s|--output=s\t\tResult namebase. Default: 'result'" << std::endl <<
    "-v n|--verbose=n\tVerbose level: " << std::endl <<
    "\t\t\t\t0 -- no output,"<< std::endl <<
    "\t\t\t\t1 -- errors only, "<< std::endl << 
    "\t\t\t\t2 -- error + warnings," << std::endl <<
    "\t\t\t\t3 -- error + warnings + info," << std::endl <<
    "\t\t\t\t4 -- full verbose."<< std::endl << 
    "\t\t\t\tDefault: info" << std::endl <<
    std::endl;
}

int main(int argc, char **argv) {
    MPI_Init(&argc,&argv);
    int param = 0;
    int option_index = -1;
    opterr = 0;
    
    char ** itr = std::find(argv, argv+argc, std::string("-v"));
    if (itr != argv+argc && ++itr != argv+argc) {
	Logger::setMode(atoi(*itr));
    }
    
    const char *shortOptions = "hm:l:r:t:c:v:o:";
    const struct option longOptions[] = {
	{"independent",no_argument,NULL,'i'},
	{"help",no_argument,NULL,'h'},
	{"time",required_argument,NULL,'m'},
	{"left",required_argument,NULL,'l'},
	{"right",required_argument,NULL,'r'},
	{"timestep",required_argument,NULL,'t'},
	{"coordstep",required_argument,NULL,'c'},
	{"times",required_argument,NULL,330},
	{"coords",required_argument,NULL,340},
	{"output",required_argument,NULL,'o'},
	{"verbose",required_argument,NULL,'v'},
	{NULL,0,NULL,0}
    };
    
    double left=0, right=1, time=1, timestep=0.001, coordstep=0.05, times=0, coords=0;
    bool independent = false;
    std::string output;
    output = "result";
    
    Logger::verbose("Parsing params");
    while ((param = getopt_long_only(argc, argv, shortOptions, longOptions, &option_index)) != -1) {
	switch (param) {
	    case '?': Logger::error("Unknown option! Read this manual carefully"); printUsage(argv[0]); return 0;
	    case 'h': Logger::verbose("Show help"); printUsage(argv[0]); return 0;
	    case 'i': independent = true;	Logger::verbose("Set time-independent to true"); break;
	    case 'm': time = atof(optarg); 	Logger::verbose("Set max time to " + std::to_string(time)); break;
	    case 'l': left = atof(optarg); 	Logger::verbose("Set left to " + std::to_string(left)); break;
	    case 'r': right = atof(optarg); 	Logger::verbose("Set right to " + std::to_string(right)); break;
	    case 't': timestep = atof(optarg); 	Logger::verbose("Set time step to " + std::to_string(timestep)); break;
	    case 'c': coordstep = atof(optarg); Logger::verbose("Set coordinate step to " + std::to_string(coordstep)); break;
	    case 330: times = atof(optarg); 	Logger::verbose("Set times count to " + std::to_string(times) + ". Time step'll be recalculated"); break;
	    case 340: coords = atof(optarg); 	Logger::verbose("Set coordinates count to " + std::to_string(coords) + ". Coordinate step'll be recalculated"); break;
	    case 'o': output = optarg; 		Logger::verbose("Set output basename to " + output); break;	    
	}
    }
    
    Logger::verbose("Create task");
    Task *task = new Task(&f, &initial, &boundaryLeft, &boundaryRight, time, left, right, timestep, coordstep, coords, times, independent);
    PlotDataPreparer writer;
    
    writer.WriteData(task->isTimeIndependent() ? TimeIndependentSolver::solve(task) : Solver::solve(task), output);
    
    MPI_Finalize();
    return 0;
}

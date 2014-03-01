#include <iostream>
#include <cmath>
#include <unistd.h>
#include "task.h"
#include "explicitheatsolver.h"
#include "implicitheatsolver.h"
#include "plotdatapreparer.h"

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
    "-t n\t\tMaximum time to calculate; default = 10" << std::endl <<
    "-l n\t\tLeft coordinate; default = 0" << std::endl << 
    "-r n\t\tRight coordinate; default = 1" << std::endl <<
    "-ts n\t\tTime step; default = 0.001" << std::endl <<
    "-cs n\t\tCoordinate step; default = 0.05. Please notice that time step depends on coordinate step in such way: ts <= 0.5 * cs^2" << std::endl <<
    "-tc n\t\tTimes count; if given, time step option will be ignored" << std::endl <<
    "-cc n\t\tCoordinates count; if given, coordinate step option will be ognored" << std::endl <<
    std::endl <<
    "output\t\tName without format. Program'll generate 'output.dat' with plot data and 'output.pl' with script that will produce 'output.gif' result plot" << std::endl <<
    std::endl;
}

int main(int argc, char **argv) {
    int param = 0;
    opterr = 0;
    while ((param = getopt(argc, argv, "h")) != -1) {
	switch (param) {
	    case '?': printUsage(argv[0]); return 0;
	    case 'h': printUsage(argv[0]); return 0;
	}
    }
    
    
    Task *task = new Task(alpha, &f, &initial, &boundaryLeft, &boundaryRight);
    PlotDataPreparer writer;
    
    writer.WriteData(ExplicitHeatSolver::solve(task), "exp");
    //preparer.WriteData(ImplicitHeatSolver::solve(task), "imp");
    return 0;
}

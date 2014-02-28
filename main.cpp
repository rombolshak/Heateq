#include <iostream>
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
    return 0;
}

double boundaryLeft(double t) {
    return 0;
}

double boundaryRight(double t) {
    return 0;
}

int main(int argc, char **argv) {
    Task *task = new Task(alpha, &f, &initial, &boundaryLeft, &boundaryRight);
    PlotDataPreparer writer;
    
    writer.WriteData(ExplicitHeatSolver::solve(task), "exp");
    //preparer.WriteData(ImplicitHeatSolver::solve(task), "imp");
    return 0;
}

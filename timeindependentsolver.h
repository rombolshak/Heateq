#ifndef TIMEINDEPENDENTSOLVER_H
#define TIMEINDEPENDENTSOLVER_H
#include "task.h"
#include "solvedata.h"

class TimeIndependentSolver
{
public:
    static SolveData* solve(Task* task);

private:
    static void bisect(double b, int count, int gridColumns, double eps1, long double precision, Task *task, double coordStep, double * x);
    static double getDiagValue(Task *task, int index, double coordStep);
    static void inverseIteration(double coordStep, int gridColumns, double eigenValue, Task *task, double threshold, double *output);
    static void calcFirstApproximation(double* eigenVector, double* diagElements, double coordStep, int totalCount, int n, Task *task);
    static void prepareTDA(int totalCount, double* f, double* c, int rank, int commSize);
    static void calcNewVector(double* c, double* f, double* calcVector, int totalCount, int rank, int commSize);
    static double calcDiff(int totalCount, double* eigenVector, double* calcVector);
};

#endif // TIMEINDEPENDENTSOLVER_H

#ifndef SOLVEDATA_H
#define SOLVEDATA_H
#include "task.h"

class SolveData
{
public:
    SolveData(Task *task, double **grid);
    Task* task;
    double** solveGrid;
};

#endif // SOLVEDATA_H

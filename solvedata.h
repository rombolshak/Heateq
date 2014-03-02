#ifndef SOLVEDATA_H
#define SOLVEDATA_H
#include "task.h"
#include <vector>

class SolveData
{
public:
    SolveData(Task* task, std::vector< std::vector< double > > grid, double yMin, double yMax);
    Task* task;
    std::vector< std::vector< double > > solveGrid;
    double yMin, yMax;
};

#endif // SOLVEDATA_H

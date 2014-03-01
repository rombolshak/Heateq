#ifndef SOLVEDATA_H
#define SOLVEDATA_H
#include "task.h"
#include <vector>

class SolveData
{
public:
    SolveData(Task* task, std::vector< std::vector< double > > grid);
    Task* task;
    std::vector< std::vector< double > > solveGrid;
};

#endif // SOLVEDATA_H

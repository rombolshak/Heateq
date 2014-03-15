#ifndef SOLVEDATA_H
#define SOLVEDATA_H
#include "task.h"
#include <vector>
#include <complex>

class SolveData
{
public:
    SolveData(Task* task, std::vector< std::vector< std::complex< double > > > grid);
    Task* task;
    std::vector< std::vector< std::complex< double > > > solveGrid;
};

#endif // SOLVEDATA_H

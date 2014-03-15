#include "solvedata.h"

SolveData::SolveData(Task* task, std::vector< std::vector< std::complex< double > > > grid)
{
    this->task = task;
    this->solveGrid = grid;
}

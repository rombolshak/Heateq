#include "solvedata.h"

SolveData::SolveData(Task* task, std::vector< std::vector< double > > grid, double yMin, double yMax)
{
    this->task = task;
    this->solveGrid = grid;
    this->yMin = yMin;
    this->yMax = yMax;
}

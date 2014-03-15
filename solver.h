#ifndef SOLVER_H
#define SOLVER_H
#include "task.h"
#include "solvedata.h"

class Solver
{
public:
    static SolveData* solve(Task* task);
};

#endif // SOLVER_H

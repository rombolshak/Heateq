#ifndef IMPLICITHEATSOLVER_H
#define IMPLICITHEATSOLVER_H
#include "task.h"
#include "solvedata.h"

class ImplicitHeatSolver
{
public:
    ImplicitHeatSolver();
    static SolveData* solve(Task* task);
};

#endif // IMPLICITHEATSOLVER_H

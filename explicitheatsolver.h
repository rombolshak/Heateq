#ifndef EXPLICITHEATSOLVER_H
#define EXPLICITHEATSOLVER_H
#include "task.h"
#include "solvedata.h"

class ExplicitHeatSolver
{
public:
    ExplicitHeatSolver();
    static SolveData* solve(Task* task);
};

#endif // EXPLICITHEATSOLVER_H

#ifndef EXPLICITHEATSOLVER_H
#define EXPLICITHEATSOLVER_H
#include "task.h"

class ExplicitHeatSolver
{
public:
    ExplicitHeatSolver();
    static void* solve(Task& task);
};

#endif // EXPLICITHEATSOLVER_H

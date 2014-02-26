#ifndef IMPLICITHEATSOLVER_H
#define IMPLICITHEATSOLVER_H
#include "task.h"

class ImplicitHeatSolver
{
public:
    ImplicitHeatSolver();
    static void* solve(Task& task);
};

#endif // IMPLICITHEATSOLVER_H

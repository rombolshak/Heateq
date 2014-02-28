#ifndef PLOTDATAPREPARER_H
#define PLOTDATAPREPARER_H
#include <iostream>
#include <string>
#include "solvedata.h"


class PlotDataPreparer
{
public:
    PlotDataPreparer();
    void WriteData(SolveData* data, std::string name, bool dispose = true);
};

#endif // PLOTDATAPREPARER_H

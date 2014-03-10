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
private:
    void writeDatafile(std::string name, SolveData* data);
    void writeScriptfile(std::string name, SolveData* data);
    void execute(std::string name);
};

#endif // PLOTDATAPREPARER_H

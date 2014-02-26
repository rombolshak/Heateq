#ifndef PLOTDATAPREPARER_H
#define PLOTDATAPREPARER_H
#include <iostream>
#include <string>


class PlotDataPreparer
{
public:
    PlotDataPreparer();
    void WriteData(void* data, std::string name);
};

#endif // PLOTDATAPREPARER_H

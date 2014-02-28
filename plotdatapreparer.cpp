#include "plotdatapreparer.h"
#include <fstream>

PlotDataPreparer::PlotDataPreparer()
{

}

void PlotDataPreparer::WriteData(SolveData* data, std::string name, bool dispose)
{
    double coordStep = data->task->getCoordStep();
    double timeStep = data->task->getTimeStep();
    int gridColumns = (data->task->getMaxCoord() - data->task->getMinCoord()) / coordStep;
    int gridRows = data->task->getMaxTime() / data->task->getTimeStep();
    
    std::ofstream datafile(name + ".dat");
    for (int x = 0; x < gridColumns; ++x) {
	datafile << (x * coordStep + data->task->getMinCoord()) << " ";
	for (int t = 0; t < gridRows; ++t) {
	    datafile << gridColumns[t][x] << " ";
	}
	datafile << std::endl;
    }
    datafile << std::endl;
    
    if (dispose) {
	for (int t = 0; t < gridRows; ++t) {
	    delete data->solveGrid[t];
	}
	delete data->solveGrid;
	delete data;
    }
}

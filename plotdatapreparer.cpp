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
    
    std::ofstream datafile((name + ".dat").c_str(), std::ofstream::out);
    for (int x = 0; x < gridColumns; ++x) {
	datafile << (x * coordStep + data->task->getMinCoord()) << " ";
	for (int t = 0; t < gridRows; ++t) {
	    datafile << data->solveGrid[t][x] << " ";
	}
	datafile << std::endl;
    }
    datafile << std::endl;
    datafile.close();
    
    if (dispose) {
	data->solveGrid.clear();
    }
}

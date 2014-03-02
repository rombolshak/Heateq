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
    
    std::ofstream scriptfile((name + ".pl").c_str(), std::ofstream::out);
    scriptfile << "#!/usr/bin/gnuplot -persist" << std::endl;
    scriptfile << "set term gif animate" << std::endl;
    scriptfile << "set output '" << name << ".gif'" << std::endl;
    scriptfile << "set xrange [" << data->task->getMinCoord() << ":" << data->task->getMaxCoord() << "]" << std::endl;
    scriptfile << "set yrange [" << data->yMin << ":" << data->yMax << "]" << std::endl;
    scriptfile << "set xlabel 'Координата' font 'Helvetica,18'" << std::endl;
    scriptfile << "set ylabel 'Температура' font 'Helvetica,18'" << std::endl;
    //scriptfile << "" << std::endl;
    //scriptfile << "" << std::endl;
    scriptfile << "do for [i=2:" << (gridRows+2) << "] {" << std::endl;
    scriptfile << "plot '" << name << ".dat' using 1:i with lines smooth csplines title sprintf(\"t=%f\", (i-2) * " << data->task->getTimeStep() << ")" << std::endl;
    scriptfile << "}" << std::endl;
    scriptfile.close();
    
    if (dispose) {
	data->solveGrid.clear();
    }
}

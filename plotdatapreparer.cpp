#include "plotdatapreparer.h"
#include "logger.h"
#include <fstream>
#include <cstdlib>
#include <string>

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
    
    const char * scriptfilename = (name + ".pl").c_str();
    std::ofstream scriptfile(scriptfilename, std::ofstream::out);
    scriptfile << "#!/usr/bin/gnuplot -persist" << std::endl;
    scriptfile << "set term gif animate" << std::endl;
    scriptfile << "set output '" << name << ".gif'" << std::endl;
    scriptfile << "set xrange [" << data->task->getMinCoord() << ":" << data->task->getMaxCoord() << "]" << std::endl;
    scriptfile << "set yrange [" << data->yMin << ":" << data->yMax << "]" << std::endl;
    scriptfile << "set xlabel 'Координата' font 'Helvetica,18'" << std::endl;
    scriptfile << "set ylabel 'Температура' font 'Helvetica,18'" << std::endl;
    //scriptfile << "" << std::endl;
    scriptfile << "progress = 0" << std::endl;
    scriptfile << "total = " << (gridRows+1) << std::endl;
    scriptfile << "do for [i=2:total] {" << std::endl;
    scriptfile << "  current = i * 100 / total" << std::endl;
    scriptfile << "  if (current > progress) {" << std::endl;
    scriptfile << "    print current, '%'" << std::endl;
    scriptfile << "    progress = current" << std::endl;
    scriptfile << "  }" << std::endl;
    scriptfile << "  plot '" << name << ".dat' using 1:i with lines smooth csplines title sprintf(\"t=%f\", (i-2) * " << data->task->getTimeStep() << ")" << std::endl;
    scriptfile << "}" << std::endl;
    scriptfile.close();
    
    std::string chmodCommand = ("`/usr/bin/which chmod` +x " + name + ".pl");
    system(chmodCommand.c_str());
    
    int gnuplotStatus = system("/usr/bin/which gnuplot");
    if (gnuplotStatus != 0) {
	Logger::warning("There is no gnuplot found. Please copy " + name + ".pl to machine with installed gnuplot and run: ./" + name + ".pl");
    }
    
    else {
	std::string gnuplotCommand = "./" + name + ".pl";
	system(gnuplotCommand.c_str());
    }
    
    if (dispose) {
	data->solveGrid.clear();
    }
}

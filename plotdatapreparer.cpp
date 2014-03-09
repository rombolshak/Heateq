#include "plotdatapreparer.h"
#include "logger.h"
#include <fstream>
#include <cstdlib>
#include <string>

extern std::string alpha_description();
extern std::string f_description();
extern std::string initial_description();
extern std::string left_description();
extern std::string right_description();


PlotDataPreparer::PlotDataPreparer()
{

}

void PlotDataPreparer::WriteData(SolveData* data, std::string name, bool dispose)
{
    double coordStep = data->task->getCoordStep();
    double timeStep = data->task->getTimeStep();
    int gridColumns = (data->task->getMaxCoord() - data->task->getMinCoord()) / coordStep;
    int gridRows = data->task->getMaxTime() / data->task->getTimeStep();
    
    std::string datafilename = name + ".dat";
    Logger::verbose("Begin writing data file: " + datafilename);
    
    std::ofstream datafile(datafilename.c_str(), std::ofstream::out);
    for (int x = 0; x < gridColumns; ++x) {
	datafile << (x * coordStep + data->task->getMinCoord()) << " ";
	for (int t = 0; t < gridRows; ++t) {
	    datafile << data->solveGrid[t][x] << " ";
	}
	
	datafile << std::endl;
    }
    
    datafile << std::endl;
    datafile.close();
    
    Logger::verbose("Datafile has been written");
    
    std::string scriptfilename = name + ".pl";
    Logger::verbose("Writing script file: " + scriptfilename);
    
    std::ofstream scriptfile(scriptfilename.c_str(), std::ofstream::out);
    scriptfile << "#!/usr/bin/gnuplot -persist" << std::endl;
    scriptfile << "set term gif animate enhanced" << std::endl;
    scriptfile << "set nokey" << std::endl;
    
    scriptfile << "set title \"u_{t} = " << alpha_description() << " * u_{xx} + " << f_description() << 
    "\\nu(x,0) = " << initial_description() << 
    "\\n{/Symbol m}_{1}(t) = " << left_description() << 
    "\\n{/Symbol m}_{2}(t) = " << right_description() << "\"" << std::endl;
    
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
    
    Logger::verbose("Chmod script file to be executable");
    std::string chmodCommand = ("`/usr/bin/which chmod` +x " + scriptfilename);
    system(chmodCommand.c_str());
    
    Logger::verbose("Check for gnuplot installed");
    int gnuplotStatus = system("/usr/bin/which gnuplot");
    if (gnuplotStatus != 0) {
	Logger::warning("There is no gnuplot found. Please copy " + scriptfilename + " to machine with installed gnuplot and run: ./" + scriptfilename);
    }
    
    else {
	Logger::verbose("Running gnuplot");
	std::string gnuplotCommand = "./" + scriptfilename;
	system(gnuplotCommand.c_str());
    }
    
    if (dispose) {
	Logger::verbose("Dispose data");
	data->solveGrid.clear();
    }
}

#include "plotdatapreparer.h"
#include "logger.h"
#include <fstream>
#include <cstdlib>
#include <string>
#include "mpi.h"

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
    writeDatafile(name, data);
    if (!data->task->isTimeIndependent()) {
        writeScriptfile(name, data);
        execute(name);
    }
    
    if (dispose) {
	Logger::verbose("Dispose data");
	data->solveGrid.clear();
    }
}

void PlotDataPreparer::writeDatafile(std::string name, SolveData* data)
{	
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if (0 == rank)
    {
	double coordStep = data->task->getCoordStep();
	double minCoord = data->task->getMinCoord();
	int gridColumns = (data->task->getMaxCoord() - minCoord) / coordStep;
	int gridRows = data->task->isTimeIndependent() ? 1 : data->task->getMaxTime() / data->task->getTimeStep();
	
    std::string realDataFileName = ".real.dat";
    std::string imagDataFileName = ".imag.dat";
    std::string absDataFileName = ".abs.dat";
    std::string taskDataFileName = ".task.dat";
    std::string potentialDataFileName = ".u.dat";
    Logger::verbose("Begin writing data files: " + realDataFileName + ", " + imagDataFileName + ", " +
                    absDataFileName + ", " + taskDataFileName + ", " + potentialDataFileName);
	
    std::ofstream realDataFile(realDataFileName.c_str(), std::ofstream::out);
    std::ofstream imagDataFile(imagDataFileName.c_str(), std::ofstream::out);
    std::ofstream absDataFile(absDataFileName.c_str(), std::ofstream::out);
    std::ofstream taskDataFile(taskDataFileName.c_str(), std::ofstream::out);
    std::ofstream potentialDataFile(potentialDataFileName.c_str(), std::ofstream::out);
	
    taskDataFile << data->task->isTimeIndependent() << " " << data->task->getMinCoord() << " " << data->task->getMaxCoord() << " " << data->task->getMaxTime() << " " <<
			data->task->getTimeStep() << " " << data->task->getCoordStep();
	
	_yMax = _yMin = (data->solveGrid[0][0]).real();
    double real, imag, absolute, potential;
	for (int x = 0; x < gridColumns; ++x) {
        realDataFile << (x * coordStep + minCoord) << " ";
        imagDataFile << (x * coordStep + minCoord) << " ";
        absDataFile << (x * coordStep + minCoord) << " ";
        potentialDataFile << (x * coordStep + minCoord) << " ";

	    for (int t = 0; t < gridRows; ++t) {
		real = (data->solveGrid[t][x]).real();
		imag = (data->solveGrid[t][x]).imag();
		absolute = sqrt(real * real + imag * imag);
        potential = data->task->calcF(x * coordStep, t * data->task->getTimeStep());
		
		_yMax = std::max<double>(_yMax, std::max<double>(real, std::max<double>(imag, absolute)));
		_yMin = std::min<double>(_yMin, std::min<double>(real, std::min<double>(imag, absolute)));
		
        realDataFile << real << " ";
        imagDataFile << imag << " ";
        absDataFile << absolute << " ";
        potentialDataFile << potential << " ";
	    }
	    
        realDataFile << std::endl;
        imagDataFile << std::endl;
        absDataFile << std::endl;
        potentialDataFile << std::endl;
	}
	
    realDataFile << std::endl;
    realDataFile.close();
	
    imagDataFile << std::endl;
    imagDataFile.close();
	
    absDataFile << std::endl;
    absDataFile.close();
	
    taskDataFile << std::endl;
    taskDataFile.close();

    potentialDataFile << std::endl;
    potentialDataFile.close();
	
	Logger::verbose("Datafiles has been written");
	
    std::string allFiles = realDataFileName + " " + imagDataFileName + " " +
            absDataFileName + " " + taskDataFileName + " " + potentialDataFileName;
    std::string archiveCommand = "tar -cf " + name + ".hof " + allFiles;// + " && rm " + allFiles;
	system(archiveCommand.c_str());
    }
}

void PlotDataPreparer::writeScriptfile(std::string name, SolveData* data)
{
    std::string scriptfilename = name + ".pl";
    Logger::verbose("Writing script file: " + scriptfilename);
    
    std::ofstream scriptfile(scriptfilename.c_str(), std::ofstream::out);
    scriptfile << "#!/usr/bin/gnuplot -persist" << std::endl;
    scriptfile << "set term gif animate enhanced" << std::endl;
    
    //scriptfile << "set title \"u_{t} = " << alpha_description() << " * u_{xx} + " << f_description() << 
    //"\\nu(x,0) = " << initial_description() << 
    //"\\n{/Symbol m}_{1}(t) = " << left_description() << 
    //"\\n{/Symbol m}_{2}(t) = " << right_description() << "\"" << std::endl;
    
    scriptfile << "set output '" << name << ".gif'" << std::endl;
    scriptfile << "set xrange [" << data->task->getMinCoord() << ":" << data->task->getMaxCoord() << "]" << std::endl;
    scriptfile << "set yrange [" << _yMin << ":" << _yMax << "]" << std::endl;
    scriptfile << "set xlabel 'Координата' font 'Helvetica,18'" << std::endl;
    //scriptfile << "set ylabel 'Температура' font 'Helvetica,18'" << std::endl;
    //scriptfile << "" << std::endl;
    scriptfile << "progress = 0" << std::endl;
    scriptfile << "total = " << ((data->task->getMaxTime() / data->task->getTimeStep())+1) << std::endl;
    scriptfile << "do for [i=2:total] {" << std::endl;
    scriptfile << "  current = i * 100 / total" << std::endl;
    scriptfile << "  if (current > progress) {" << std::endl;
    scriptfile << "    print current, '%'" << std::endl;
    scriptfile << "    progress = current" << std::endl;
    scriptfile << "  }" << std::endl;
    scriptfile << "  plot '.real.dat' using 1:i with lines smooth csplines ti 'real part' ,\\" << std::endl;
    scriptfile << "  '.imag.dat' using 1:i with lines smooth csplines ti 'imag part' ,\\" << std::endl;
    scriptfile << "  '.abs.dat' using 1:i with lines smooth csplines ti 'abs'" << std::endl;
    scriptfile << "}" << std::endl;
    scriptfile.close();
    Logger::verbose("Scriptfile has been written");
    
    Logger::verbose("Chmod script file to be executable");
    std::string chmodCommand = ("`/usr/bin/which chmod` +x " + scriptfilename);
    system(chmodCommand.c_str());
}

void PlotDataPreparer::execute(std::string name)
{
    std::string scriptfilename = name + ".pl";
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
}

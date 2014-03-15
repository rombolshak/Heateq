#include "solver.h"
#include "logger.h"
#include <vector>
#include <algorithm>
#include <complex>

SolveData* Solver::solve(Task* task)
{
    Logger::info("Start implicit scheme solver");
    
    double coordStep = task->getCoordStep();
    double timeStep = task->getTimeStep();
    int gridColumns = (task->getMaxCoord() - task->getMinCoord()) / coordStep;
    int gridRows = task->getMaxTime() / task->getTimeStep();
    
    std::vector< std::vector< std::complex<double> > > grid (gridRows, std::vector< std::complex< double > >(gridColumns, std::complex<double>(0)));
    Logger::verbose("Grid allocated");
    
    grid[0][0] = task->calcLeftBoundary(0);
    grid[0][gridColumns - 1] = task->calcRightBoundary(task->getMaxTime());
    for (int x = 1; x < gridColumns - 1; ++x) {
	grid[0][x] = task->calcInitial(x * coordStep + task->getMinCoord());
    }
    
    Logger::verbose("Initial values computed");
    
    double gamma = timeStep / (coordStep * coordStep);
    int n = gridColumns - 2;
    std::complex<double> a(0, -gamma / 2);
    std::complex<double> b(0, -gamma / 2);
    
    for (int t = 1; t < gridRows; ++t) {
	
	std::vector< std::complex<double> > c(n, 0);
	std::vector< std::complex<double> > f(n, 0);
    
	grid[t][0] = task->calcLeftBoundary(t);
	grid[t][gridColumns - 1] = task->calcRightBoundary(t);
	
	std::complex<double> iUnit(0, timeStep), onePlusGammaI(1, gamma);
	for (int i = 0; i < n; ++i) {
	    c[i] = onePlusGammaI - iUnit * task->calcF(i + 1, t);
	}
	
	f[0] = -a * grid[t][0];
	f[n-1] = -b * grid[t][gridColumns - 1];
	
	for (int i = 0; i < n; ++i) {
	    f[i] += grid[t - 1][i + 1];
	}
	
	std::complex<double> m;
	for (int i = 1; i < n; ++i) {
	    m = a / c[i - 1];
	    c[i] -= m * b;
	    f[i] -= m * f[i - 1];
	}
	
	grid[t][n-1 + 1] = f[n-1] / c[n-1];
	for (int i = n-2; i >= 0; --i) {
	    grid[t][i + 1] = (f[i] - b * grid[t][i+1 + 1]) / c[i];
	}
    }
    
    Logger::verbose("Grid completely computed");
    return new SolveData(task, grid);
}

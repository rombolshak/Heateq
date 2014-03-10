#include "implicitheatsolver.h"
#include "logger.h"
#include <vector>
#include <algorithm>

ImplicitHeatSolver::ImplicitHeatSolver()
{

}

SolveData* ImplicitHeatSolver::solve(Task* task)
{
    Logger::info("Start implicit scheme solver");
    
    double coordStep = task->getCoordStep();
    double timeStep = task->getTimeStep();
    int gridColumns = (task->getMaxCoord() - task->getMinCoord()) / coordStep;
    int gridRows = task->getMaxTime() / task->getTimeStep();
    
    std::vector< std::vector< double > > grid (gridRows, std::vector<double>(gridColumns, 0));
    Logger::verbose("Grid allocated");
    
    grid[0][0] = task->calcLeftBoundary(0);
    grid[0][gridColumns - 1] = task->calcRightBoundary(task->getMaxTime());
    for (int x = 1; x < gridColumns - 1; ++x) {
	grid[0][x] = task->calcInitial(x * coordStep + task->getMinCoord());
    }
    
    Logger::verbose("Initial values computed");
    
    double gamma = timeStep / (coordStep * coordStep);
    int n = gridColumns - 2;
    double a = -gamma;
    double b = -gamma;
    
    for (int t = 1; t < gridRows; ++t) {
	
	std::vector<double> c(n, 1 + 2 * gamma);
	std::vector<double> f(n, 0);
    
	grid[t][0] = task->calcLeftBoundary(t);
	grid[t][gridColumns - 1] = task->calcRightBoundary(t);
	
	f[0] = gamma * grid[t][0];
	f[n-1] = gamma * grid[t][gridColumns - 1];
	
	for (int i = 0; i < n; ++i) {
	    f[i] += grid[t - 1][i + 1] + timeStep * task->calcF(i + 1, t);
	}
	
	double m;
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
    
    Logger::verbose("Complete grid computed");
    std::vector<double> vMin(gridRows), vMax(gridRows);
    for (int i = 0; i < gridRows; ++i) {
	vMin[i] = (*std::min_element((grid[i]).begin(), (grid[i]).end()));
	vMax[i] = (*std::max_element((grid[i]).begin(), (grid[i]).end()));
    }
    
    double yMin = (*std::min_element(vMin.begin(), vMin.end()));
    double yMax = (*std::max_element(vMax.begin(), vMax.end()));
    
    Logger::verbose("Found min and max temperature values: " + std::to_string(yMin) + " and " + std::to_string(yMax) + " respectively");
    return new SolveData(task, grid, yMin, yMax);
}

#include "implicitheatsolver.h"
#include "logger.h"
#include <vector>
#include <algorithm>

ImplicitHeatSolver::ImplicitHeatSolver()
{

}

SolveData* ImplicitHeatSolver::solve(Task* task)
{
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
    for (int t = 1; t < gridRows; ++t) {
	grid[t][0] = task->calcLeftBoundary(t);
	grid[t][gridColumns - 1] = task->calcRightBoundary(t);
	
	/** 
	    * @see http://ru.wikibooks.org/wiki/%D0%9F%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%BD%D1%8B%D0%B5_%D1%80%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4%D0%B0_%D0%BF%D1%80%D0%BE%D0%B3%D0%BE%D0%BD%D0%BA%D0%B8
	    * n = gridColumns - 2
	    * a[i] = -gamma for all i
	    * b[i] = -gamma for all i
	    * c[i] = (1 + 2*gamma) for all i
	    * f[i] = grid[t-1][i] + timeStep * task->calcF(i, t)
	    * x[i] = grid[t][i]
	    */
	int n = gridColumns - 2;
	double a = -gamma;
	double b = -gamma;
	std::vector<double> c(n, 1 + 2 * gamma);
	
	std::vector<double> f(n, 0);
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

#include "explicitheatsolver.h"
#include "logger.h"
#include <iostream>
#include <vector>
#include <algorithm>

ExplicitHeatSolver::ExplicitHeatSolver()
{

}

SolveData* ExplicitHeatSolver::solve(Task* task)
{
    double coordStep = task->getCoordStep();
    double timeStep = task->getTimeStep();
    int gridColumns = (task->getMaxCoord() - task->getMinCoord()) / coordStep;
    int gridRows = task->getMaxTime() / task->getTimeStep();
    
    std::vector< std::vector< double > > grid (gridRows, std::vector<double>(gridColumns, 0));
        
    grid[0][0] = task->calcLeftBoundary(0);
    grid[0][gridColumns - 1] = task->calcRightBoundary(task->getMaxTime());
    for (int x = 1; x < gridColumns - 1; ++x) {
	grid[0][x] = task->calcInitial(x * coordStep + task->getMinCoord());
    }
    
    double gamma = timeStep / (coordStep * coordStep);    
    for (int t = 1; t < gridRows; ++t) {
	grid[t][0] = task->calcLeftBoundary(t);
	grid[t][gridColumns - 1] = task->calcRightBoundary(t);    
	for (int x = 1; x < gridColumns - 1; ++x) {
	    grid[t][x] = grid[t-1][x] + gamma * (grid[t-1][x-1] - 2 * grid[t-1][x] + grid[t-1][x+1]) + timeStep * task->calcF(x, t);
	}
    }
    
    std::vector<double> vMin(gridRows), vMax(gridRows);
    for (int i = 0; i < gridRows; ++i) {
	vMin[i] = (*std::min_element((grid[i]).begin(), (grid[i]).end()));
	vMax[i] = (*std::max_element((grid[i]).begin(), (grid[i]).end()));
    }
    
    double yMin = (*std::min_element(vMin.begin(), vMin.end()));
    double yMax = (*std::max_element(vMax.begin(), vMax.end()));
    
    return new SolveData(task, grid, yMin, yMax);
}


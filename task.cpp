#include "task.h"
#include "logger.h"
#include <algorithm>
#include <sstream>

Task::Task(std::complex<double> (*f)(double, double), 
	   std::complex<double> (*initial)(double),
	   std::complex<double> (*boundaryLeft)(double),
	   std::complex<double> (*boundaryRight)(double),
	   double maxTime,
	   double minCoord,
	   double maxCoord,
	   double timeStep,
	   double coordStep,
	   double coordStepsCount,
	   double timeStepsCount)
{
    _f = f;
    _initial = initial;
    _boundaryLeft = boundaryLeft;
    _boundaryRight = boundaryRight;
    _maxTime = maxTime;
    _minCoord = minCoord;
    _maxCoord = maxCoord;
    
    if (_maxCoord - _minCoord < 1e-6) {
	Logger::error("Coordinates difference too small. Nothing to compute. Quit");
	exit(1);
    }
    
    if (_maxTime <= 0) {
	Logger::error("Unacceptable time");
	exit(1);
    }
    
    if (coordStepsCount < 0) {
	Logger::warning("Coords count negative. Assume you meant " + std::to_string(-coordStepsCount));
	coordStepsCount = -coordStepsCount;
    }
    
    if (timeStepsCount < 0) {
	Logger::warning("Times count negative. Assume you meant " + std::to_string(-timeStepsCount));
	timeStepsCount = -timeStepsCount;
    }
    
    if (_initial(minCoord) != _boundaryLeft(0) || _initial(maxCoord) != _boundaryRight(0)) {
	Logger::warning("Initial and boundaries conditions are NOT conclusive. Will use initial values at t = 0 even at boundary points");
    }
    
    _coordStep = coordStepsCount == 0 ? coordStep : (maxCoord - minCoord) / coordStepsCount;
    _timeStep = timeStepsCount == 0 ? timeStep : maxTime / timeStepsCount;
    
    double maxTimeStep = 0.5 * _coordStep * _coordStep;
    if (maxTimeStep < _timeStep) {
	Logger::warning("Time step has inappropiate value. It was shrank to " + std::to_string(maxTimeStep));
	_timeStep = maxTimeStep;
    }
    
    std::ostringstream strs;
    strs << "Created task with following specifications:" << std::endl <<
    "Time from 0 to " << _maxTime << std::endl <<
    "Coordinates from " << _minCoord <<  " to " << _maxCoord << std::endl <<
    "Timestep is " << _timeStep << ", total times count is " << (_maxTime / _timeStep) << std::endl <<
    "Coordstep is " << _coordStep << ", total coordinates count is " << ((_maxCoord - _minCoord) / _coordStep) << std::endl;
    Logger::info(strs.str());    
}

std::complex<double> Task::calcF(double x, double t)
{
    return _f(x, t);
}

std::complex<double> Task::calcInitial(double x)
{
    return _initial(x);
}

std::complex<double> Task::calcLeftBoundary(double t)
{
    return _boundaryLeft(t);
}

std::complex< double > Task::calcRightBoundary(double t)
{
    return _boundaryRight(t);
}

double Task::getMaxTime()
{
    return _maxTime;
}

double Task::getMinCoord()
{
    return _minCoord;
}

double Task::getMaxCoord()
{
    return _maxCoord;
}

double Task::getTimeStep()
{
    return _timeStep;
}

double Task::getCoordStep()
{
    return _coordStep;
}

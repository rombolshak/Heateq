#include "task.h"

Task::Task(double alpha,
	   double (*f)(double, double), 
	   double (*initial)(double),
	   double (*boundaryLeft)(double),
	   double (*boundaryRight)(double),
	   double maxTime,
	   double minCoord,
	   double maxCoord,
	   double timeStep,
	   double coordStep)
{
    _alpha = alpha;
    _f = f;
    _initial = initial;
    _boundaryLeft = boundaryLeft;
    _boundaryRight = boundaryRight;
    _maxTime = maxTime;
    _minCoord = minCoord;
    _maxCoord = maxCoord;
    _timeStep = timeStep;
    _coordStep = coordStep;
}

double Task::getAlpha()
{
    return _alpha;
}

double Task::calcF(double x, double t)
{
    return _f(x, t);
}

double Task::calcInitial(double x)
{
    return _initial(x);
}

double Task::calcLeftBoundary(double t)
{
    return _boundaryLeft(t);
}

double Task::calcRightBoundary(double t)
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

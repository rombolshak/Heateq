#include "task.h"

Task::Task(double alpha,
	   double (*f)(double, double) const, 
	   double (*initial)(double) const,
	   double (*boundaryLeft)(double) const,
	   double (*boundaryRight)(double) const)
{
    _alpha = alpha;
    _f = f;
    _initial = initial;
    _boundaryLeft = boundaryLeft;
    _boundaryRight = boundaryRight;
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

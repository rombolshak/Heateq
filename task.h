#ifndef TASK_H
#define TASK_H

class Task
{
public:
    Task(
	 double alpha,
	 double (*f)(double, double), 
	 double (*initial)(double),
	 double (*boundaryLeft)(double),
	 double (*boundaryRight)(double),
	 double maxTime = 10,
	 double minCoord = 0,
	 double maxCoord = 1,
	 double timeStep = 0.001,
	 double coordStep = 0.1,
	 double coordStepsCount = 0,
	 double timeStepsCount = 0
	);
    
    double getAlpha();
    double calcF(double x, double t);
    double calcInitial(double x);
    double calcLeftBoundary(double t);
    double calcRightBoundary(double t);
    double getMaxTime();
    double getMinCoord();
    double getMaxCoord();
    double getTimeStep();
    double getCoordStep();
    
private:
    double _alpha;
    double (*_boundaryRight)(double);
    double (*_boundaryLeft)(double);
    double (*_initial)(double);
    double (*_f)(double, double);
    double _maxTime;
    double _minCoord;
    double _maxCoord;
    double _timeStep;
    double _coordStep;
};

#endif // TASK_H

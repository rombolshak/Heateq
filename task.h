#ifndef TASK_H
#define TASK_H
#include <complex>

class Task
{
public:
    Task(
     double (*f)(double, double),
	 std::complex<double> (*initial)(double),
	 std::complex<double> (*boundaryLeft)(double),
	 std::complex<double> (*boundaryRight)(double),
	 double maxTime = 1, 
	 double minCoord = 0, 
	 double maxCoord = 1, 
	 double timeStep = 0.001, 
	 double coordStep = 0.05, 
	 double coordStepsCount = 0, 
	 double timeStepsCount = 0, 
	 bool timeIndependent = false
	);
    
    double calcF(double x, double t);
    std::complex<double> calcInitial(double x);
    std::complex<double> calcLeftBoundary(double t);
    std::complex<double> calcRightBoundary(double t);
    double getMaxTime();
    double getMinCoord();
    double getMaxCoord();
    double getTimeStep();
    double getCoordStep();    
    bool isTimeIndependent();
    
private:
    std::complex<double> (*_boundaryRight)(double);
    std::complex<double> (*_boundaryLeft)(double);
    std::complex<double> (*_initial)(double);
    double (*_f)(double, double);
    double _maxTime;
    double _minCoord;
    double _maxCoord;
    double _timeStep;
    double _coordStep;
    bool _timeIndependent;
};

#endif // TASK_H

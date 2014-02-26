#ifndef TASK_H
#define TASK_H

class Task
{
public:
    Task(
	 double alpha,
	 double (*f)(double, double) const, 
	 double (*initial)(double) const,
	 double (*boundaryLeft)(double) const,
	 double (*boundaryRight)(double) const);
    
    double getAlpha();
    double calcF(double x, double t);
    double calcInitial(double x);
    double calcLeftBoundary(double t);
    double calcRightBoundary(double t);
    
private:
    double _alpha;
    double (*_boundaryRight)(double);
    double (*_boundaryLeft)(double);
    double (*_initial)(double);
    double (*_f)(double, double);
};

#endif // TASK_H

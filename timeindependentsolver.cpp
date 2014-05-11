#include "timeindependentsolver.h"
#include "logger.h"
#include <mpi.h>
#include <cmath>
#include <limits>
#include <string>

void TimeIndependentSolver::bisect(double b, int count, int gridColumns, double eps1, long double precision, Task *task, double coordStep, double * x) {
    int rank, commSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);

    int n = gridColumns / commSize;
    int additionalElementsCount = 0;
    if (rank == commSize - 1) {
        additionalElementsCount = gridColumns % commSize;
    }
    int totalCount = n + additionalElementsCount;

    double * c = new double[n + additionalElementsCount];
    for (int i = 0; i < n + additionalElementsCount; ++i) {
        c[i] = getDiagValue(task, n * rank + i, coordStep);
    }

    double beta = b * b;

    double xmin = c[totalCount-1] - std::abs(b);
    double xmax = c[totalCount-1] + std::abs(b);

    #pragma omp parallel for reduction(max:xmax) reduction(min, xmin)
    for (int i = totalCount-2; i >= 0; --i) {
        double h = std::abs(b) + std::abs(b);
        if (c[i] + h > xmax) {
            xmax = c[i] + h;
        }
        if (c[i] - h < xmin) {
            xmin = c[i] - h;
        }
    }
    MPI_Allreduce(MPI_IN_PLACE, &xmin, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE, &xmax, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    double * wu = new double[count];

    double x0 = xmax;
    for (int i = 0; i < count; ++i) {
        x[i] = xmax;
        wu[i] = xmin;
    }

    // loop for k-th eigenvalue
    for (int k = count - 1; k >= 0; --k) {
        double xu = xmin;
        for (int i = k; i >= 0; --i) {
            if (xu < wu[i]) {
                xu = wu[i];
                break;
            }
        }

        if (x0 > x[k]) {
            x0 = x[k];
        }

        while( x0 - xu > 2 * precision * (std::abs(xu) + std::abs(x0)) + eps1) {
            double x1 = (xu + x0) / 2;
            int a = 0;
            double q = 1;
            if (0 != rank) {
                MPI_Recv(&q, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            for (int i = 0; i < totalCount; ++i) {
                q = c[i] - x1 - (q != 0 ? (0 == rank && 0 == i ? 0 : beta / q) : std::abs(b)/precision);
                if (q < 0) {
                    a += 1;
                }
            }
            if (rank != commSize - 1) {
                MPI_Send(&q, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
            }

            MPI_Allreduce(MPI_IN_PLACE, &a, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

            if (a < k) {
                xu = wu[a + 1] = x1;
                if (x[a] > x1) {
                    x[a] = x1;
                }
            }
            else {
                x0 = x1;
            }
        }

        x[k] = (x0 + xu) / 2;
    }

    delete[] wu;
}

void TimeIndependentSolver::calcFirstApproximation(double* eigenVector, double* diagElements, double coordStep, int totalCount, int n, Task *task)
{
    int rank, commSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);

    double norm = 0;
    if (0 == rank) {
        eigenVector[0] = 1;
        eigenVector[1] = diagElements[0];
        norm = 1 + eigenVector[1] * eigenVector[1];
    }
    else {
        double prev, double_prev;
        MPI_Recv(&double_prev, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&prev, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        eigenVector[0] = -1. * double_prev + getDiagValue(task, rank * n - 1, coordStep) * prev;
        eigenVector[1] = -1. * prev + diagElements[0] * eigenVector[0];
        norm += eigenVector[0] * eigenVector[0] + eigenVector[1] * eigenVector[1];
    }

    for (int i = 1; i < totalCount - 1; ++i) {
        eigenVector[i + 1] = -1. * eigenVector[i - 1] + diagElements[i] * eigenVector[i];
        norm += eigenVector[i + 1] * eigenVector[i + 1];
    }

    if (rank != commSize - 1) {
        MPI_Send(&eigenVector[totalCount - 2], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&eigenVector[totalCount - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Allreduce(MPI_IN_PLACE, &norm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    norm = sqrt(norm);
    if (isnan(norm)) {
        Logger::error("Something went wrong, maybe your grid is extremely big, or potential function returns 'strange' values that are over9000");
        exit(2);
    }

    #pragma omp parallel for
    for (int i = 0; i < totalCount; ++i) {
        eigenVector[i] /= norm;
    }
}

void TimeIndependentSolver::prepareTDA(int totalCount, double* f, double* c, int rank, int commSize)
{
    if (rank != 0) {
        double prev_c, prev_f;
        MPI_Recv(&prev_c, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&prev_f, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        double m = -1 / prev_c;
        c[0] += m;
        f[0] -= m * prev_f;
    }
    for (int i = 1; i < totalCount; ++i) {
        double m = -1 / c[i - 1];
        c[i] += m;
        f[i] -= m * f[i - 1];
    }
    if (rank != commSize - 1) {
        MPI_Send(&c[totalCount - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&f[totalCount - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    }
}

void TimeIndependentSolver::calcNewVector(double* c, double* f, double* calcVector, int totalCount, int rank, int commSize)
{
    double calcNorm = 0;
    if (rank == commSize - 1) {
        calcVector[totalCount - 1] = f[totalCount - 1] / c[totalCount - 1];
        calcNorm += calcVector[totalCount - 1] * calcVector[totalCount - 1];
    }
    else {
        double next;
        MPI_Recv(&next, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        calcVector[totalCount - 1] = (f[totalCount - 1] + next) / c[totalCount - 1];
    }
    for (int i = totalCount - 2; i >= 0; --i) {
        calcVector[i] = (f[i] + calcVector[i + 1]) / c[i];
        calcNorm += calcVector[i] * calcVector[i];
    }
    if (rank != 0) {
        MPI_Send(&calcVector[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
    }

    MPI_Allreduce(MPI_IN_PLACE, &calcNorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    calcNorm = sqrt(calcNorm);

    #pragma omp parallel for
    for (int i = totalCount - 1; i >= 0; --i) {
        calcVector[i] /= calcNorm;
    }
}

double TimeIndependentSolver::calcDiff(int totalCount, double* eigenVector, double* calcVector)
{
    double diff = 0;

    #pragma omp parallel for reduction(+:diff)
    for (int i = 0; i < totalCount; ++i) {
        diff += 2 * eigenVector[i] * calcVector[i];
    }

    MPI_Allreduce(MPI_IN_PLACE, &diff, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    diff = 2 - diff;
    diff = sqrt(std::abs(diff));
    return diff;
}

void TimeIndependentSolver::inverseIteration(double coordStep, int gridColumns, double eigenValue, Task *task, double threshold, double *output)
{
    int rank, commSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    int n = gridColumns / commSize;
    int additionalElementsCount = 0;
    if (rank == commSize - 1) {
        additionalElementsCount = gridColumns % commSize;
    }
    int totalCount = n + additionalElementsCount;

    double * eigenVector = new double[totalCount];
    double * calcVector = new double[totalCount];
    double * diagElements = new double[totalCount];
    double * f = new double[totalCount];
    double * c = new double[totalCount];

    #pragma omp parallel for
    for (int i = 0; i < totalCount; ++i) {
        diagElements[i] = getDiagValue(task, rank * n + i, coordStep) - eigenValue;
    }

    calcFirstApproximation(eigenVector, diagElements, coordStep, totalCount, n, task);
    for (int iteration = 1;;++iteration) {
        memcpy(c, diagElements, totalCount * sizeof(double));

        #pragma omp parallel for
        for (int i = 0; i < totalCount; ++i) {
            f[i] = eigenVector[i] / eigenValue;
        }

        prepareTDA(totalCount, f, c, rank, commSize);
        calcNewVector(c, f, calcVector, totalCount, rank, commSize);
        double diff = calcDiff(totalCount, eigenVector, calcVector);

        if ((diff < threshold) || (std::abs(diff - 2) < threshold)) {
            Logger::verbose("Diff: " + std::to_string(diff * 1e10));
            Logger::verbose("Threshold: " + std::to_string(threshold * 1e10));
            Logger::verbose("Iteration: " + std::to_string(iteration));
            break;
        }

        double *qwe = eigenVector;
        eigenVector = calcVector;
        calcVector = qwe;
    }

    MPI_Gather(eigenVector, n, MPI_DOUBLE, output, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    delete[] f;
    delete[] c;
    delete[] calcVector;
    delete[] eigenVector;
    delete[] diagElements;
}

SolveData* TimeIndependentSolver::solve(Task *task)
{
    Logger::info("Start time-independent solver");

    bool timeIndependent = task->isTimeIndependent();
    if (!timeIndependent) {
        Logger::error("Using time-independent solver for time-dependent task is not appropriate");
        throw 42;
    }

    double coordStep = task->getCoordStep();
    int gridColumns = (task->getMaxCoord() - task->getMinCoord()) / coordStep;
    int gridRows = 1;

    std::vector< std::vector< std::complex<double> > > grid = std::vector< std::vector< std::complex<double> > > (gridRows, std::vector< std::complex< double > >(gridColumns, std::complex<double>(0)));
    Logger::verbose("Grid allocated");

    double * eigenValues = new double[4];
    double * eigenVector = new double[gridColumns];
    double threshold = 1e-7;

    bisect(-1, 4, gridColumns, threshold, std::numeric_limits<double>::epsilon(), task, coordStep, eigenValues);

    double eigenValue = eigenValues[3];
    Logger::info("Obtained eigen value: " + std::to_string(eigenValue));

    inverseIteration(coordStep, gridColumns, eigenValue, task, threshold, eigenVector);

    #pragma omp parallel for
    for (int i = 0; i < gridColumns; ++i) {
        grid[0][i] = eigenVector[i];
    }

    delete[] eigenValues;
    return new SolveData(task, grid);
}

double TimeIndependentSolver::getDiagValue(Task *task, int index, double coordStep) {
    return 2. + task->calcF(index * coordStep, 0) * coordStep * coordStep;
}

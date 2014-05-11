CC=mpiCC
CFLAGS= -g -std=c++0x
LDFLAGS= 
SRC=solver.cpp timeindependentsolver.cpp logger.cpp plotdatapreparer.cpp solvedata.cpp task.cpp main.cpp
EXECUTABLE=heateq

DESCRIBER=describe_task.sh
TASK_CPP=task_definition.cpp task_defines.cpp

all: $(SRC) $(DESCRIBER)
	chmod +x $(DESCRIBER)
	./$(DESCRIBER)
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASK_CPP) $(SRC) -o $(EXECUTABLE)

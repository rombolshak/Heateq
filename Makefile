CC=g++
CFLAGS= -g -O3 -std=c++0x 
LDFLAGS= 
SRC=explicitheatsolver.cpp implicitheatsolver.cpp logger.cpp plotdatapreparer.cpp solvedata.cpp task.cpp main.cpp
EXECUTABLE=heateq

DESCRIBER=describe_task.sh
TASK_CPP=task_definition.cpp task_defines.cpp

heateq: $(SRC) $(DESCRIBER)
	chmod +x $(DESCRIBER)
	./$(DESCRIBER)
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASK_CPP) $(SRC) -o $(EXECUTABLE)

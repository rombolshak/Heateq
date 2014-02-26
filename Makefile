CC=g++
CFLAGS= -g -O3
SRC=*.cpp
EXECUTABLE=heateq

heateq: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXECUTABLE)
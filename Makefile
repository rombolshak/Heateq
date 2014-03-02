CC=g++
CFLAGS= -g -O3
LDFLAGS= 
SRC=*.cpp
EXECUTABLE=heateq

heateq: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(EXECUTABLE)
CC=g++
CFLAGS= -g -O3
LDFLAGS= -lpthread -lncurses
SRC=*.cpp
EXECUTABLE=heateq

heateq: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(EXECUTABLE)
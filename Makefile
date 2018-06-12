BUILD_FLAGS = -W -Wall -pedantic 
CC = g++
RM = rm
EXES = *.exe
OBJECTS = *.o

all: Driver.exe

Ch6Ex2.o: Makefile ConwaysGame.cpp
	$(CC) $(BUILD_FLAGS) -c ConwaysGame.cpp -o ConwaysGame.o


Driver.exe: Makefile ConwaysGame.o main.cpp 
	$(CC) $(BUILD_FLAGS) ConwaysGame.o main.cpp -o Driver.exe 

clean:
	$(RM) $(EXES) $(OBJECTS)
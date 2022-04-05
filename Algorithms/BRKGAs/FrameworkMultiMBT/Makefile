# Name of the project
PROJ_NAME=framework_multi_mbt

# Flags CPP
CFLAGS=-std=c++11 -static-libstdc++ -static-libgcc -Wall -O3

# Source files
DECODERS_SOURCE=$(wildcard ../Decoders/*.cpp)
TIMER_SOURCE=$(wildcard ../Timer/*.cpp)
GRAPH_SOURCE=$(wildcard ../Graph/*.cpp)
MBT_SOURCE=$(wildcard ../MBT/*.cpp)
IN_SOURCE=$(wildcard *.cpp)

ALL_SOURCE=$(GRAPH_SOURCE) $(MBT_SOURCE) $(DECODERS_SOURCE) $(TIMER_SOURCE) $(IN_SOURCE) 

# Object files
OBJ=$(ALL_SOURCE:.cpp=.o)

all:$(OBJ)
	g++  $(OBJ) -o $(PROJ_NAME) $(CPLEXFLAGS) $(CFLAGS)

%.o: %.cpp %.hpp
	g++ -c $< -o $@ $(CFLAGS)

main.o: main.cpp
	g++ -c -o main.o main.cpp $(CPLEXFLAGS) $(CFLAGS)

clean:
	rm -rf ../Decoders/*.o
	rm -rf ../Graph/*.o
	rm -rf ../MBT/*.o
	rm -rf ../Timer/*.o
	rm -rf *.o $(PROJ_NAME) *~

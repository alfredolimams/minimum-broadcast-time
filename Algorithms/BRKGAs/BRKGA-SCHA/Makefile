CFLAGS=-O3 -fopenmp -static-libstdc++ -static-libgcc -Wall

all:main.o MBTDecoder.o Forest.o Graph.o ArgPack.o BossaTimer.o
	g++  main.o MBTDecoder.o Forest.o Graph.o ArgPack.o BossaTimer.o -o main.run $(CPLEXFLAGS) $(CFLAGS)

%.o: %.cpp %.hpp
	g++ -c $< -o $@ $(CFLAGS)

main.o: main.cpp
	g++ -c -o main.o main.cpp $(CPLEXFLAGS) $(CFLAGS)

clean:
	rm -f *.o
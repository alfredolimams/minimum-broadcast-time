#include <bits/stdc++.h>
#include "ArgPack.hpp"
#include "../MBTLibrary/Timer/BossaTimer.hpp"
#include "../MBTLibrary/Graph/UnweightedGraph.hpp"
#include "ACS.hpp"

#define DEBUG if(0)

using namespace std;

int main(int argc, char * argv[])
{
	ArgPack args = ArgPack(argc, argv);
	BossaTimer timer = BossaTimer();
	// Variables
	ofstream outputFile;
	ifstream inputFile;

	int qtVertex;
	int qtEdges;
	int qtGateway;
	int v, u;
	int population;

	if( !args.Ok() ){
		cerr << "[MAIN] There is problem with arguments!" << endl;
		return -1;
	}

	if( args.writeFile ){
		// Setting first answer (not working yet)
		outputFile.open(args.outputFile, ios::out);

		if( !outputFile ){
			cerr << "[MAIN] Unable to open output file with name: " << args.outputFile << endl;
			exit(1);
		}
		outputFile << "-1" << endl; 
		outputFile.close();
	}

	inputFile.open(args.inputFile, ios::in);
	if( !inputFile ){
		cerr << "[MAIN] Unable to open input file with name: " << args.inputFile << endl;
		exit(1);
	}

	timer.setMaxTime(args.t);

	try {
		inputFile >> qtVertex >> qtGateway >> qtEdges;
		population = 2 * qtVertex;
		UnweightedGraph graph = UnweightedGraph(qtVertex);

		for (int i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v;
			graph.addEdge(u-1,v-1);	
		}

		for (int i = 0; i < qtGateway; ++i) {
			inputFile >> u;
			graph.addSource(u-1);
		}

		srand(args.rngSeed);

		ACS ACS(&graph, &timer);
		timer.start();
		BroadcastSchedule answer = ACS.MBT(args.generations, population, args.tal, 
											args.alpha, args.beta, args.rho, args.mutants);

		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
			outputFile << "MBT: " << answer.getMBT() << endl;
			outputFile << "Time to best answer: " << timer.getLap() << endl;
			outputFile << "Time total: " << timer.getTime() << endl;
			// outputFile << "Iterations: " << generation << endl;
			outputFile << "Solution: " << endl << answer << endl;
			outputFile.close();
		}

		if( args.printOutput ){
			cout << "MBT: " << answer.getMBT() << endl;
			cout << "Time to best answer: " << timer.getLap() << endl;
			cout << "Time total: " << timer.getTime() << endl;
			// cout << "Iterations: " << generation << endl;
			cout << "Solution: " << endl << answer << endl;
		}

	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}
	return 0;
}
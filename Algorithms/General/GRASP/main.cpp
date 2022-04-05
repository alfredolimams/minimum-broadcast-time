#include <bits/stdc++.h>
#include "MTRand.hpp"
#include "ArgPack.hpp"
#include "../../MBTLibrary/Graph/WeightedGraph.hpp"
#include "../../MBTLibrary/Graph/WeightedForest.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"

#include "Grasp.hpp"

using namespace std;

int main(int argc, char * argv[])
{
	ArgPack args = ArgPack(argc, argv);
	BossaTimer timer = BossaTimer();

	// Variables
	ofstream outputFile;
	ifstream inputFile;

	int qtVertices;
	int qtEdges;
	int qtGateway;
	int v, u, w;

	// int length_cromosome;
	if( !args.Ok() ){
		cerr << "[MAIN] There is problem with arguments!" << endl;
		return -1;
	}

	inputFile.open(args.inputFile, ios::in);
	if( !inputFile ){
		cerr << "[MAIN] Unable to open input file with name: " << args.inputFile << endl;
		exit(1);
	}

	try {
		MTRand rng(args.rngSeed); // Initialize random number generator	

		inputFile >> qtVertices >> qtGateway >> qtEdges;

		vector<int> weights_vertices(qtVertices);
		for (int i = 0; i < qtVertices ; ++i) {
			inputFile >> weights_vertices[i];
		}

		WeightedGraph graph = WeightedGraph(qtVertices, weights_vertices);
		for (int i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v >> w;
			graph.addEdge(u-1,v-1, w);	
		}

		for (int i = 0; i < qtGateway; ++i) {
			inputFile >> u;
			graph.addSource(u-1);
		}
		
		GRASP method = GRASP(&graph, &rng, &timer);

		WeightedForest answer; 

		answer = method.compute(args.generations, args.t, args.alpha);
		
		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
		}

		if( args.printOutput ){
			cout << "MBT: " << answer.MBT() << endl;
		}

		if( args.writeFile ){
			outputFile << "MBT: " << answer.MBT() << endl;
		}

		if( args.writeFile ){
			outputFile.close();
		}

	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}

	return 0;
}
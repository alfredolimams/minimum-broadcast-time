#include <bits/stdc++.h>
#include "ArgPack.hpp"
#include "../../MBTLibrary/Graph/WeightedGraph.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"
#include "Harutyunyan.hpp"

using namespace std;

int main(int argc, char * argv[])
{
	ArgPack args = ArgPack(argc, argv);
	// Variables
	ofstream outputFile;
	ifstream inputFile;
	BossaTimer timer = BossaTimer();

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
		timer.start();
		HarutyunyanGreedy greedy = HarutyunyanGreedy(&graph);
		int mbt = greedy.mbt();
		timer.pause();
		
		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
		}

		if( args.printOutput ){
			cout << "MBT: " << mbt << endl;
			cout << "Time: " << timer.getTime() << endl;
			//cout << "Schedule:" << endl << broadcastSchedule;
		}

		if( args.writeFile ){
			outputFile << "MBT: " << mbt << endl;
			outputFile << "Time: " << timer.getTime() << endl;
			//outputFile << "Schedule:" << endl << broadcastSchedule;
		}

		if( args.writeFile ){
			outputFile.close();
		}

	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}

	return 0;
}
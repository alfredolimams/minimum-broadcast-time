#include <bits/stdc++.h>
#include "ArgPack.hpp"
#include "../../MBTLibrary/Graph/WeightedForest.hpp"
#include "../../MBTLibrary/MBT/BroadcastSchedule.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"

using namespace std;

int main(int argc, char * argv[])
{
	ArgPack args = ArgPack(argc, argv);
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
		inputFile >> qtVertices >> qtGateway >> qtEdges;

		vector<int> weights_vertices(qtVertices);
		for (int i = 0; i < qtVertices ; ++i) {
			inputFile >> weights_vertices[i];
		}

		WeightedForest forest = WeightedForest(qtVertices, weights_vertices);
		for (int i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v >> w;
			forest.addEdge(u-1,v-1, w);	
		}

		for (int i = 0; i < qtGateway; ++i) {
			inputFile >> u;
			forest.addRoot(u-1);
		}
	
		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
		}

		BroadcastSchedule broadcastSchedule = BroadcastSchedule(forest);

		if( args.printOutput ){
			cout << "MBT: " << broadcastSchedule.getMBT() << endl;
			cout << "Schedule:" << endl << broadcastSchedule;
		}

		if( args.writeFile ){
			outputFile << "MBT: " << broadcastSchedule.getMBT() << endl;
			outputFile << "Schedule:" << endl << broadcastSchedule;
		}

		if( args.writeFile ){
			outputFile.close();
		}
		
	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}

	return 0;
}
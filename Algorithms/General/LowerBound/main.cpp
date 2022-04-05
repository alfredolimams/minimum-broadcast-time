#include <bits/stdc++.h>
#include "ArgPack.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"
#include "../../MBTLibrary/Graph/UnweightedGraph.hpp"

#define DEBUG if(0)

using namespace std;

int main(int argc, char * argv[])
{
	ArgPack args = ArgPack(argc, argv);
	// Variables
	ofstream outputFile;
	ifstream inputFile;

	int qtVertex;
	int qtEdges;
	int qtGateway;
	int v, u;

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

	try {
		inputFile >> qtVertex >> qtGateway >> qtEdges;
		UnweightedGraph graph = UnweightedGraph(qtVertex);

		for (int i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v;
			graph.addEdge(u-1,v-1);	
		}

		for (int i = 0; i < qtGateway; ++i) {
			inputFile >> u;
			graph.addSource(u-1);
		}

		int lb = graph.LBB_BFS();

		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
			outputFile << "LB: " << lb << endl;
			outputFile.close();
		}

		if( args.printOutput ){
			cout << "LB: " << lb << endl;
		}

	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}
	return 0;
}
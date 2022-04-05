#include <bits/stdc++.h>
#include "ArgPack.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"
#include "../../MBTLibrary/Graph/WeightedGraph.hpp"

#define DEBUG if(0)

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
	
		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
		}

		int lb = graph.LBB();
		vector<int> dist = graph.getDist();
		if( args.writeFile ){
			outputFile << "LB: " << lb << endl;
			for (int i = 0; i < qtVertices; ++i) {
				outputFile << "V_" << i+1 << ": " << dist[i] << endl; 
			}
		}

		if( args.printOutput ){
			cout << "LB: " << lb << endl;
			for (int i = 0; i < qtVertices; ++i) {
				cout << "V_" << i+1 << ": " << dist[i] << endl; 
			}
		}

		if( args.writeFile ){
			outputFile.close();
		}
		
	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}
	return 0;
}
#include <bits/stdc++.h>
#include "DiTree.hpp"
#include "BossaTimer.hpp"
#include "ArgPack.hpp"

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
		inputFile >> qtVertices >> qtEdges;
		DiTree tree = DiTree(qtVertices);
		for (int i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v;
			tree.addEdge(u-1,v-1);	
		}
		inputFile >> u;
		tree.setRoot(u-1);

		timer.start();
		int mbt = tree.MBT();
		timer.pause();

		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
			outputFile << "MBT: " << mbt << endl;
			outputFile << "Time total: " << timer.getTime() << endl;
			outputFile.close();
		}

		if( args.printOutput ){
			cout << "MBT: " << mbt << endl;
			cout << "Time total: " << timer.getTime() << endl;
		}

	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}

	return 0;
}
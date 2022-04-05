#include <bits/stdc++.h>
#include "../BRKGA_API/MTRand.hpp"
#include "../BRKGA_API/BRKGA.hpp"
#include "ArgPack.hpp"
#include "../Graph/Graph.hpp"
#include "../Timer/BossaTimer.hpp"
#include "../Decoders/Decoders.hpp"

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
	int v, u;

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
		// cout << "[MAIN] Read graph caracteristics" << endl;
		Graph graph = Graph(qtVertices);
		// cout << "[MAIN] Read edges" << endl;
		for (int i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v;
			graph.addEdge(u-1,v-1);	
		}

		// cout << "[MAIN] Read gateways" << endl;
		for (int i = 0; i < qtGateway; ++i) {
			inputFile >> u;
			graph.addSource(u-1);
		}
		int lbb = graph.LBB_BFS();
		MTRand rng(args.rngSeed); // Initialize random number generator	
		timer.setMaxTime(args.t);
		vector<Decoder_MBT *> decoders;
		for (unsigned i = 0; i < args.decoders ; ++i) {
			DecoderM(decoders,i,args);
		}

		unsigned generation;
		unsigned qt_decoders = decoders.size();
		for (unsigned i = 0; i < qt_decoders ; ++i) {
			if( args.printOutput ){
				cout << "## \tDECODER " << i << "\t ##" << endl; 
			}
			if( args.writeFile ){
				outputFile.open(args.outputFiles[i], ios::out);
			}
			generation = 1; // Current generation
			decoders[i]->init(&graph, &timer, max(lbb, args.result));
			BRKGA<Decoder_MBT, MTRand> brkga(qtVertices, args.population, decoders[i]->getPe(), decoders[i]->getPm(), decoders[i]->getRhoe(), *decoders[i], rng, args.K, args.threads);
			if( args.printOutput && args.verbose ) cout << "[" << generation << "] " << brkga.getBestFitness() << endl;
			if( args.writeFile && args.verbose ) outputFile << "[" << generation << "] " << brkga.getBestFitness() << endl;
			while(generation < args.generations && !decoders[i]->stop()){
				brkga.evolve(); // Evolve the population for one generation
				generation++;
				if( args.printOutput && args.verbose ) cout << "[" << generation << "] " << brkga.getBestFitness() << endl;
				if( args.writeFile && args.verbose ) outputFile << "[" << generation << "] " << brkga.getBestFitness() << endl;
			}
			timer.pause();
			if( args.printOutput ){
				cout << "MBT: " << decoders[i]->getBestAnswer() << endl;
				cout << "Time to best answer: " << decoders[i]->getTimeAnswer() << endl;
				cout << "Time total: " << timer.getTime() << endl;
				cout << "Iterations: " << generation << endl;
				cout << "Solution: " << endl << decoders[i]->broadcast_schedule(brkga.getBestChromosome()) << endl;
			}
			if( args.writeFile ){
				outputFile << "MBT: " << decoders[i]->getBestAnswer() << endl;
				outputFile << "Time to best answer: " << decoders[i]->getTimeAnswer() << endl;
				outputFile << "Time total: " << timer.getTime() << endl;
				outputFile << "Iterations: " << generation << endl;
				outputFile << "Solution: " << endl << decoders[i]->broadcast_schedule(brkga.getBestChromosome()) << endl;
				outputFile.close();
			}
		}

		decoders.clear();
	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}

	return 0;
}
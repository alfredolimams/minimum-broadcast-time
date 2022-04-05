#include <bits/stdc++.h>
#include "MBTDecoder.hpp"
#include "MTRand.hpp"
#include "BRKGA.hpp"
#include "ArgPack.hpp"

#define DEBUG if(0)

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
			graph.addGateway(u-1);
		}

		MTRand rng(args.rngSeed); // Initialize random number generator	
		MBTDecoder decoder = MBTDecoder(); // Initialize decoder
		decoder.init(&graph, &timer);
		timer.setMaxTime(args.t);


		// // Initialize BRKGA-based heuristic
		BRKGA<MBTDecoder, MTRand> algorithm(	qtVertices, 
												args.population, 
												args.populationElite, 
												args.populationMutants, 
												args.rhoe, 
												decoder, 
												rng, 
												args.K, 
												args.threads);
		
		// cout << "[MAIN] Init BRKGA with decoder" << endl;

		unsigned generation = 1; // Current generation
		
		// Iterations of the algorithm ...
		timer.start();

		// cout << "[MAIN] Run BRKGA" << endl;
		do{
			algorithm.evolve(); // Evolve the population for one generation
			if((++generation) % args.exchangeBest == 0){
				algorithm.exchangeElite(args.exchangeTop); // Exchange top individuals
			}
		} while (generation < args.generations && !decoder.stop());

		// cout << "[MAIN] BRKGA Finished" << endl;

		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
			outputFile << "MBT: " << algorithm.getBestFitness() << endl;
			outputFile << "Time to best answer: " << timer.getLap() << endl;
			outputFile << "Time total: " << timer.getTime() << endl;
			outputFile.close();
		}

		if( args.printOutput ){
			cout << "MBT: " << algorithm.getBestFitness() << endl;
			cout << "Time to best answer: " << timer.getLap() << endl;
			cout << "Time total: " << timer.getTime() << endl;
		}

	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}

	return 0;
}
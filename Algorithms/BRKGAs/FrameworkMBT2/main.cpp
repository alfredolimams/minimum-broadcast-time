#include <bits/stdc++.h>
#include "ArgPack.hpp"
#include "../BRKGA_API/MTRand.hpp"
#include "../BRKGA_API/BRKGA.hpp"
#include "../Decoders/Decoders.hpp"
#include "../../MBTLibrary/Graph/WeightedGraph.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"

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
		// cout << "[MAIN] Read graph caracteristics" << endl;
		vector<int> weights_vertices(qtVertices);
		for (int i = 0; i < qtVertices ; ++i) {
			cin >> weights_vertices[i];
		}
		WeightedGraph graph = WeightedGraph(qtVertices, weights_vertices);
		// cout << "[MAIN] Read edges" << endl;
		for (int i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v >> w;
			graph.addEdge(u-1,v-1, w);	
		}

		// cout << "[MAIN] Read gateways" << endl;
		for (int i = 0; i < qtGateway; ++i) {
			inputFile >> u;
			graph.addSource(u-1);
		}
		
		// int lbb = graph.LBB_BFS();
		MTRand rng(args.rngSeed); // Initialize random number generator	
		timer.setMaxTime(args.t);
		Decoder_MBT * decoder;
		// DecoderS(decoder,args);

		unsigned generation;
		if( args.printOutput ){
			cout << "## \tDECODER " << args.typeDecoder << "\t ##" << endl; 
		}
		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
		}

		// generation = 1; // Current generation
		// decoder->init(&graph, &timer, max(lbb, args.result));
		// // length_cromosome = args.typeDecoder == "MST" ? qtEdges : qtVertices;
		// BRKGA<Decoder_MBT, MTRand> brkga(qtVertices, args.population, decoder->getPe(), decoder->getPm(), decoder->getRhoe(), *decoder, rng, args.K, args.threads);
		// if( args.printOutput && args.verbose ) cout << "[" << generation << "] " << brkga.getBestFitness() << endl;
		// if( args.writeFile && args.verbose ) outputFile << "[" << generation << "] " << brkga.getBestFitness() << endl;
		// while(generation < args.generations && !decoder->stop()){
		// 	brkga.evolve(); // Evolve the population for one generation
		// 	generation++;
		// 	if( args.printOutput && args.verbose ) cout << "[" << generation << "] " << brkga.getBestFitness() << endl;
		// 	if( args.writeFile && args.verbose ) outputFile << "[" << generation << "] " << brkga.getBestFitness() << endl;
		// }
		// timer.pause();
		// if( args.printOutput ){
		// 	cout << "MBT: " << decoder->getBestAnswer() << endl;
		// 	cout << "Time to best answer: " << decoder->getTimeAnswer() << endl;
		// 	cout << "Time total: " << timer.getTime() << endl;
		// 	cout << "Iterations: " << generation << endl;
		// 	cout << "Solution: " << endl << decoder->broadcast_schedule(brkga.getBestChromosome()) << endl;
		// }
		// if( args.writeFile ){
		// 	outputFile << "MBT: " << decoder->getBestAnswer() << endl;
		// 	outputFile << "Time to best answer: " << decoder->getTimeAnswer() << endl;
		// 	outputFile << "Time total: " << timer.getTime() << endl;
		// 	outputFile << "Iterations: " << generation << endl;
		// 	outputFile << "Solution: " << endl << decoder->broadcast_schedule(brkga.getBestChromosome()) << endl;
		// 	outputFile.close();
		// }
		
	} catch (...) {
		cerr << "[MAIN] Unknown exception caught!" << endl;
	}

	return 0;
}
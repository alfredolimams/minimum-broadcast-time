#include <bits/stdc++.h>
#include "../BRKGA_API/MTRand.hpp"
#include "../BRKGA_API/BRKGA.hpp"
#include "ArgPack.hpp"
#include "../Graph/Graph.hpp"
#include "../Graph/Forest.hpp"
#include "../Timer/BossaTimer.hpp"
#include "../MBT/BroadcastSchedule.hpp"
#include "../Decoders/Decoders.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	ArgPack args = ArgPack(argc, argv);
	BossaTimer timer = BossaTimer();
	// Variables
	ofstream outputFile;
	ifstream inputFile;

	if( !args.Ok() ){
		cerr << "[MAIN] There is problem with arguments!" << endl;
		exit(1);
	}

	inputFile.open(args.inputFile, ios::in);
	if( !inputFile ){
		cerr << "[MAIN] Unable to open input file with name: " << args.inputFile << endl;
		exit(1);
	}

	try {
		unsigned qtVertices;
		unsigned qtEdges;
		unsigned qtSources;
		unsigned v, u;

		inputFile >> qtVertices >> qtSources >> qtEdges;
		Graph graph = Graph(qtVertices);

		for (unsigned i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v;
			graph.addEdge(u-1,v-1);	
		}

		for (unsigned i = 0; i < qtSources; ++i) {
			inputFile >> u;
			graph.addSource(u-1);
		}

		int lbb = max(graph.LBB_BFS(), args.result);
		MTRand rng(args.rngSeed); // Initialize random number generator	
		timer.setMaxTime(args.t);  
		Decoder_MBT * decoder;
		DecoderS(decoder,args);

		unsigned generation;
		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
		}
		generation = 1; // Current generation
		decoder->init(&graph, &timer, lbb);
		BRKGA<Decoder_MBT, MTRand> brkga(qtEdges, args.population, decoder->getPe(), decoder->getPm(), decoder->getRhoe(), *decoder, rng, args.K, args.threads);
		if( args.printOutput && args.verbose ) cout << "[" << generation << "] " << brkga.getBestFitness() << endl;
		if( args.writeFile && args.verbose ) outputFile << "[" << generation << "] " << brkga.getBestFitness() << endl;
		while(generation < args.generations && !decoder->stop()){
			brkga.evolve(); // Evolve the population for one generation
			generation++;
			if( args.printOutput && args.verbose ) cout << "[" << generation << "] " << brkga.getBestFitness() << endl;
			if( args.writeFile && args.verbose ) outputFile << "[" << generation << "] " << brkga.getBestFitness() << endl;
		}		
		timer.pause();
		BroadcastSchedule answer_schedule = decoder->broadcast_schedule(brkga.getBestChromosome());
		if( args.printOutput ){
			cout << "MBT: " << decoder->getBestAnswer() << endl;
			cout << "Time to best answer: " << decoder->getTimeAnswer() << endl;
			cout << "Time total: " << timer.getTime() << endl;
			cout << "Iterations: " << generation << endl;
			cout << "Schedule:" << endl << answer_schedule << endl;
		}

		if( args.writeFile ){
			outputFile << "MBT: " << decoder->getBestAnswer() << endl;
			outputFile << "Time to best answer: " << decoder->getTimeAnswer() << endl;
			outputFile << "Time total: " << timer.getTime() << endl;
			outputFile << "Iterations: " << generation << endl;
			outputFile << "Schedule:" << endl << answer_schedule << endl;
			outputFile.close();
		}
		
	} catch (...)	{
		cerr << "Unknown exception caught!" << endl;
	}
	
	return 0;
}
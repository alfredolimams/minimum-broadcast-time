#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>
#include "../BRKGA_API/MTRand.hpp"
#include "../BRKGA_API/BRKGA.hpp"
#include "ArgPack.hpp"
#include "../../MBTLibrary/Graph/UnweightedGraph.hpp"
#include "../../MBTLibrary/Graph/Forest.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"
#include "../../MBTLibrary/MBT/BroadcastSchedule.hpp"
#include "../Decoders/Decoders.hpp"

using namespace std;

#define eps 1e-5

BroadcastSchedule ILP_MBT(UnweightedGraph *graph, BroadcastSchedule *initial, int lb, int ub, int threads, double timeLimit){

	// Indexs to edges
	int qtVertices = graph->numberVertices();
	int qtSources = graph->getSources().size();
	int lowerBound = max(graph->LBB_BFS(), lb);
	if ( lowerBound == ub ){
		return *initial;
	}
	unordered_map<int,unordered_map<int,int>> indexs;
	vector<bool> K(qtVertices, false);
	int Tmax = ub+1;
	BroadcastSchedule schedule;

	for (auto s : graph->getSources()) {
		K[s] = true;
	}

	IloEnv env;
	try {
		// Start model
		IloModel KK_MBTProblem(env, "MBT Problem");
		IloCplex cplex(KK_MBTProblem);

		if( timeLimit > 0 ){
			cplex.setParam(IloCplex::TiLim, timeLimit);
		}
		cplex.setParam(IloCplex::Threads, threads);
		// cplex.setParam(IloCplex::TreLim, 16*1024);
		cplex.setParam(IloCplex::Param::MIP::Display, 0);

		// Decision Variables: Domain
		//	-	x[time][from][to]
		IloArray<IloArray<IloNumVarArray>> x(env, Tmax);
		for (int t = 0; t < Tmax; t++) {
			x[t] = IloArray<IloNumVarArray>(env, qtVertices);
			for (int i = 0; i < qtVertices; i++) {
				x[t][i] = IloNumVarArray(env, qtVertices, 0, 1, ILOBOOL);
				for (int j = 0; j < qtVertices; j++) {
					KK_MBTProblem.add( x[t][i][j] );
				}
			}
		}

	    // Initial Solution
		if( initial ){
			IloNumVarArray startVar(env);
			IloNumArray startVal(env);
			IloArray<IloArray<IloNumArray>> xi(env, Tmax);
			
			for (int t = 0; t < Tmax; t++) {
				xi[t] = IloArray<IloNumArray>(env, qtVertices);
				for (int i = 0; i < qtVertices; i++) {
					xi[t][i] = IloNumArray(env, qtVertices);
					for (int j = 0; j < qtVertices; j++) {
						xi[t][i][j] = 0;
					}
				}
			}

			int time = 0;

			for ( auto transmitions : initial->getSchedule() ){
				time++;
				for ( auto e : transmitions ){
					xi[time][e.first][e.second] = 1;
				}
			}

			for (int t = 0; t < Tmax; t++) {
				for (int i = 0; i < qtVertices; i++) {
					for (int j = 0; j < qtVertices; j++) {
						startVar.add(x[t][i][j]);
						startVal.add(xi[t][i][j]);
					}
				}
			}

			cplex.addMIPStart(startVar, startVal, IloCplex::MIPStartAuto, "Initial solution");
			xi.end();
			startVal.end();
			startVar.end();
		}

	    IloIntVar z(env, lowerBound, Tmax);

		// // Contraints
		//	- (2) Todos os vértices devem receber a informação		
		for (int v = 0; v < qtVertices; v++) {
			IloExpr received(env);
			for (int t = 1; t < Tmax; t++) {
				for ( auto u : graph->getNeighbors(v) ){
					received += x[t][u][v];
				}
			}
			received += K[v];
			KK_MBTProblem.add(received == 1);		
		}

		//	- (3) Each vertex can send until a data at each time slot t
		for (int t = 1; t < Tmax; t++) {
			for (int v = 0; v < qtVertices; v++) {
				IloExpr send(env);
				for ( auto u : graph->getNeighbors(v) ){
					send += x[t][v][u];
				}
				KK_MBTProblem.add(send <= 1);
			}
		}

		// - (4) Transmite only, case it has received previously.
		for (int T = 1; T < Tmax; T++) {
			for (int i = 0; i < qtVertices; i++) {
				for (auto j : graph->getNeighbors(i) ){	
					IloExpr sum(env);
					for (int t = 0; t < T; t++) {
						for (auto k : graph->getNeighbors(i) ){
							if( k != j ){
								sum += x[t][k][i];
							}
						}
					}
					KK_MBTProblem.add( x[T][i][j] <= sum + K[i] );	
				}
			}
		}

		//	- (5) Value of T must be greater or equal the time of any transmittion 
		for (int t = 1; t < Tmax; t++) {
			for ( auto e : graph->getEdges() ){
				KK_MBTProblem.add( z >= t * (x[t][e.first][e.second] + x[t][e.second][e.first]));			
			}
		}

		// 	- (1) Objective Function:
		KK_MBTProblem.add(IloMinimize(env, z));

		// // More constraints for to help the model 
		// - (1) If the vertex  distance of any source greater t, the its edges are set to 0 (zero). 
		vector<int> dist = graph->getDist();
		for (int i = 0; i < qtVertices ; ++i) {
			for ( auto v : graph->getNeighbors(i) ) {
				for ( int t = 0 ; t <= dist[i] ; ++t ){
					KK_MBTProblem.add( x[t][i][v] == 0 );	
				}
			}
		}

		// - (2) Verify if a forest
		IloExpr sum(env);		
		for (int t = 1; t < Tmax; t++) {
			for ( auto e : graph->getEdges() ){
				sum += x[t][e.first][e.second] + x[t][e.second][e.first];			
			}
		}
		KK_MBTProblem.add( sum + qtSources == qtVertices );		
		sum.end();

		if ( cplex.solve() ){
			int answer = cplex.getObjValue();
			IloArray<IloArray<IloNumArray>> solutions(env, Tmax);
			for (int t = 0; t < Tmax; t++) {
				solutions[t] = IloArray<IloNumArray>(env, qtVertices);
				for (int i = 0; i < qtVertices; i++) {
					solutions[t][i] = IloNumArray(env,qtVertices);
					cplex.getValues(solutions[t][i], x[t][i]);					
				}
			}
			vector<vector<int>> ordering(qtVertices);
			for (int t = 1; t <= answer; t++) {
 				for (int i = 0; i < qtVertices ; ++i) {
					for (int j = 0; j < qtVertices; ++j){
						if( solutions[t][i][j] > 0 ) {
							ordering[i].push_back(j);
						}
					}
				}
			}
			schedule = BroadcastSchedule(graph->getSources(), ordering);
		}
	} catch (const IloException& e) {
		cerr << "Exception caught: " << e << endl;
	}
	catch (...)	{
		cerr << "Unknown exception caught!" << endl;
	}		
	env.end();
	return schedule;
}

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
		unsigned population;
		inputFile >> qtVertices >> qtSources >> qtEdges;
		population = qtVertices;
		UnweightedGraph graph = UnweightedGraph(qtVertices);

		unsigned improvement = 550U - (unsigned)(qtVertices/10);
		improvement = min(improvement, 500U);
		improvement = max(improvement, 20U);

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
		generation = 0; // Current generation
		decoder->init(&graph, &timer, lbb);
		BRKGA<Decoder_MBT, MTRand> brkga(qtVertices, population, decoder->getPe(), decoder->getPm(), decoder->getRhoe(), *decoder, rng, args.K, args.threads);
		int noImprovement = 0;
		if( args.printOutput && args.verbose ) cout << "[" << generation << "] " << brkga.getBestFitness() << endl;
		if( args.writeFile && args.verbose ) outputFile << "[" << generation << "] " << brkga.getBestFitness() << endl;
		BroadcastSchedule answer_schedule = decoder->broadcast_schedule(brkga.getBestChromosome());
		while(generation < args.generations && !decoder->stop()){
			brkga.evolve(); // Evolve the population for one generation
			generation++;
			noImprovement++;
			if( brkga.getBestFitness() < answer_schedule.getMBT() ){
				answer_schedule = decoder->broadcast_schedule(brkga.getBestChromosome());
				noImprovement = 0;
			}
			if( args.printOutput && args.verbose ) cout << "[" << generation << "] " << brkga.getBestFitness() << endl;
			if( args.writeFile && args.verbose ) outputFile << "[" << generation << "] " << brkga.getBestFitness() << endl;
			if( noImprovement%improvement == 0 && !decoder->stop() && noImprovement ){
				if( args.printOutput && args.verbose ) cout << "[MAIN] Build subgraph" << endl;
				bool buildSubgraph = true;
				noImprovement = 0;
				// Residual graph
				UnweightedGraph subgraph(qtVertices);
				set<pair<int,int>> edges;
				for (unsigned i = 0; i < population && buildSubgraph ; ++i) {
					BroadcastSchedule schedule = decoder->broadcast_schedule(brkga.getPopulation().getChromosome(i));
					for ( auto transmitions : schedule.getSchedule() ){
						for ( auto e : transmitions ){
							int e_min = min(e.first,e.second);
							int e_max = max(e.first,e.second);
							if ( !edges.count({e_min,e_max}) ) {
								edges.insert({e_min,e_max});
								subgraph.addEdge(e_min, e_max);
								if (i && subgraph.getDensity() > args.density) buildSubgraph = false;
								if (!buildSubgraph) break;
							}
						}
						if (!buildSubgraph) break;
					}
				}

				for ( auto s : graph.getSources() ){
					subgraph.addSource(s);
				}

				// Best schedule 
				BroadcastSchedule schedule_brkga = decoder->broadcast_schedule(brkga.getBestChromosome());
				// Max ILP's runtime
				double ilp_time = min(args.MaxETime, args.t - timer.getTime());
				if ( ilp_time < 5 )
					continue;
				// Run ILP Model
				if( args.printOutput && args.verbose ) cout << "[MAIN] Call ILP." << endl;
				BroadcastSchedule schedule_ilp = ILP_MBT(&subgraph, &schedule_brkga, lbb, brkga.getBestFitness(), args.threads, ilp_time);
				// Check if ILP finds better solution 
				if ( schedule_ilp.getMBT() < brkga.getBestFitness() ) {
					if( args.printOutput && args.verbose ) cout << "[MAIN] ILP found a better answer." << endl;
					decoder->setBestAnswer(schedule_ilp.getMBT());
					vector<double> chromosome = decoder->broadcast_to_cromossome(schedule_ilp);
					brkga.setChromosome(0, 0, chromosome, schedule_ilp.getMBT());
				}
			}
		}
		
		timer.pause();
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
		
	} catch (const IloException& e) {
		cerr << "Exception caught: " << e << endl;
	}
	catch (...)	{
		cerr << "Unknown exception caught!" << endl;
	}
	
	return 0;
}
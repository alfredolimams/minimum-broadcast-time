#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>
using namespace std;

#define DEBUG if(0)
#define INF 0x3f3f3f

struct Vertex
{
	int x, y, c;
};

static void show_usage(string name)
{
    cerr << "Usage: " << name << " <option(s)> SOURCES"
			<< "Options:\n"
			<< "\t-h,--help\t\tShow this help message\n"
			<< "\t-i,--input Input file\tSpecify the destination path\n"
			<< "\t-o,--output Output file\tSpecify the destination path"
			<< endl;
}

int main(int argc, char *argv[])
{
	string input;
	string output;

	/*
		Parameters
	*/

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];
		if( arg == "-h" or arg == "-help" ){
			show_usage(argv[0]);
			return 0;
		} else if(arg == "-i" or arg == "-input") {
			try {
				input = argv[++i];
			} 
			catch (...)	{
				cerr << "Unknown exception caught!" << endl;
			}
		} else if (arg == "-o" or arg == "-output"){
			try {
				output = argv[++i];
			} 
			catch (...)	{
				cerr << "Unknown exception caught!" << endl;
			}
		} else {
			cerr << "Unexpected parameter!" << endl;
			return 0;
		}
	}

	if( input == "" ){
		cerr << "Missing parameters!" << endl;
		return 0;
	}

	if( output == "" ){
		output =  input + "_E1" ;
	}

	input += ".in";
	output += ".out";

	/*
		Mathematical modeling
	*/

	IloEnv env;
	try {
		// Variables
		ofstream outputFile;
		ifstream inputFile;
		int qtVertex, qtEdges, qtUpdated, asnwerHeuristic;
		int u, v;
		string name;
		vector<set<int>> graph;
		vector<pair<int,int>> edges;
		set<int> updateds;
		vector<Vertex> vertex;

		// Setting first answer (not working yet)
		outputFile.open(output, ios::out);

		if( !outputFile ){
			cerr << "Unable to open output file with name: " << output << endl;
			exit(1);
		}
		outputFile << "-1" << endl; 
		outputFile.close();

		// Start model
		IloModel KK_MBTProblem(env, "KK-MBT Problem");
		IloCplex cplex(KK_MBTProblem);

		// Getting information about of graph
		inputFile.open(input, ios::in);
		if( !inputFile ){
			cerr << "Unable to open input file with name: " << input << endl;
			exit(1);
		}
		inputFile >> qtVertex >> qtUpdated >> qtEdges; 
		asnwerHeuristic = qtVertex;

		DEBUG cout << "## ------------------- ##" << endl; 
		DEBUG cout << "Qt vertex: " << qtVertex << endl; 
		DEBUG cout << "Qt updated: " << qtUpdated << endl; 
		DEBUG cout << "Qt edges: " << qtEdges << endl; 
		DEBUG cout << "Asnwer Heuristic: " << asnwerHeuristic << endl; 
		DEBUG cout << "## ------------------- ##" << endl; 

		graph.resize(qtVertex);
		edges.resize(qtEdges);
		vertex.resize(qtVertex);

		for (int i = 0; i < qtEdges ; i++) {
			inputFile >> u >> v;
			graph[u-1].insert(v-1);
			graph[v-1].insert(u-1);
			edges[i] = {u-1,v-1};
		}

		for (int i = 0; i < qtUpdated ; i++) {
			inputFile >> v;
			updateds.insert(v-1);
		}

		// Decision Variables: Domain
		//	-	x[time][from][to]
		IloArray<IloArray<IloIntVarArray>> x(env, qtVertex);
		for (int t = 0; t < asnwerHeuristic; t++) {
			x[t] = IloArray<IloIntVarArray>(env, qtVertex);
			for (int i = 0; i < qtVertex; i++) {
				x[t][i] = IloIntVarArray(env, qtVertex, 0, 1);
				for (int j = 0; j < qtVertex; j++) {
					KK_MBTProblem.add( x[t][i][j] );
				}
			}
		}

		//	-	z
	    IloIntVar z(env, 0, 1<<20);

		// // Contraints
		// - (2) Vértices que estão atuaizados em T = 0
		for (int i = 0; i < qtVertex; i++) {
			for (int j = 0; j < qtVertex; j++) {
				if( i == j ) continue;
				KK_MBTProblem.add(x[0][i][j] == 0);
			}
			KK_MBTProblem.add(x[0][i][i] == updateds.count(i));
		}

		for (int i = 0; i < qtVertex; i++) {
			for (int j = 0; j < qtVertex; j++) {
				if( !graph[i].count(j) ){
					for (int t = 1; t < asnwerHeuristic; t++) {
						KK_MBTProblem.add(x[t][i][j] == 0);			
					}
				}
			}
		}

		//	- (3) Todos os vértices devem receber as informações uma única vez
		for (int i = 0; i < qtVertex; i++) {
			IloExpr times(env);
			for (int t = 0; t < asnwerHeuristic; t++) {
				for ( auto j : graph[i] ){
					times += x[t][j][i];
				}
			}
			times += x[0][i][i];
			KK_MBTProblem.add(times == 1);
		}

		for (auto e : edges) {
			IloExpr used(env);			
			for (int t = 1; t < asnwerHeuristic; t++) {
				used += x[t][e.first][e.second];
				used += x[t][e.second][e.first];
			}
			KK_MBTProblem.add( used <= 1 );
		}
		

		//	- (4) Transmite em cada tempo até C_i vezes
		for (int i = 0; i < qtVertex; i++) {
			for ( int t = 0 ; t < asnwerHeuristic; ++t ) {
				IloExpr times(env);
				for ( auto j : graph[i] ){
					times += x[t][i][j];
				}
				KK_MBTProblem.add(times <= 1);
			}
		}

		// - (5) Transmite a atualização somente se já recebeu
		for (int T = 1; T < asnwerHeuristic; T++) {
			for (int i = 0; i < qtVertex; i++) {
				for (auto j : graph[i] ){	
					IloExpr sum(env);
					for (int t = 0; t < T; t++) {
						for (auto k : graph[i] ){
							if( k != j ){
								sum += x[t][k][i];
							}
						}
					}
					KK_MBTProblem.add( x[T][i][j] <= sum + x[0][i][i]);
				}
			}
		}

		//	- (6) 
		for (int t = 0; t < asnwerHeuristic; t++) {
			for (int i = 0; i < qtVertex; i++) {
				for (int j = 0; j < qtVertex; j++) {
					KK_MBTProblem.add( z >= t * x[t][i][j]);			
				}
			}
		}
		
		// // 	- (1) Objective Function:
		KK_MBTProblem.add(IloMinimize(env, z));

		const clock_t begin_time = clock();
		cplex.solve();
		float run_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
		int minBroadcast = cplex.getObjValue();
		
		outputFile.open(output, ios::out);
		
		outputFile << "MBT: " << minBroadcast << endl;
		outputFile << "Time: " << run_time << endl;
		outputFile.close();

	} catch (const IloException& e) {
		cerr << "Exception caught: " << e << endl;
	}
	catch (...)	{
		cerr << "Unknown exception caught!" << endl;
	}		
	env.end();
	return(0);
}
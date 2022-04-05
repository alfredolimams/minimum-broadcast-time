#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>
using namespace std;

#define DEBUG if(1)

struct Edge
{
	int v;
	int idx;
};

int main()
{
	IloEnv env;
	try {
		IloModel KK_MBTProblem(env, "KK-MBT Problem");
		IloCplex cplex(KK_MBTProblem);

		int qtVertex, qtEdges, qtUpdated;
		int u, v;
		string name;
		vector<vector<Edge>> graph;
		vector<pair<int,int>> edges;
		set<int> updateds;
		vector<int> capacity;

		cin >> qtVertex >> qtEdges >> qtUpdated;
		graph.resize(qtVertex);
		edges.resize(2*qtEdges);
		capacity.resize(qtVertex);

		for (int i = 0; i < qtVertex ; i++) {
			cin >> capacity[i];
		}

		for (int i = 0; i < qtUpdated ; i++) {
			cin >> v;
			updateds.insert(v);
		}

		for (int i = 0; i < qtEdges ; i++) {
			cin >> u >> v;
			graph[u].push_back({v,2*i});
			graph[v].push_back({u,2*i+1});
			edges[2*i] = {u,v};
			edges[2*i+1] = {v,u};
		}

		// Decision Variables: Domain
		//	-	x[time][vertex]
		IloArray<IloIntVarArray> x(env, qtVertex);
		for (int t = 0; t < qtVertex; t++) {
			x[t] = IloIntVarArray(env, qtVertex, 0, 1);
			for (int i = 0; i < qtVertex ; ++i) {
				KK_MBTProblem.add(x[t][i]);
			}
		}
		//	-	y[time][edge]
		IloArray<IloIntVarArray> y(env, qtVertex);
		for (int t = 0; t < qtVertex; t++) {
			y[t] = IloIntVarArray(env, 2*qtEdges, 0, 1);
			for (int i = 0; i < 2*qtEdges ; ++i) {
				KK_MBTProblem.add(y[t][i]);
			}
		}

		//	-	z
	    IloIntVar z(env, 0, 1<<20);

		// // Contraints
		// - (2) Vértices que estão atuaizados em T = 0
		for (int i = 0; i < qtVertex; i++) {
			KK_MBTProblem.add(x[0][i] == updateds.count(i));
		}
		
		for (int i = 0; i < 2*qtEdges; i++) {
			KK_MBTProblem.add(y[0][i] == 0);
		}

		//	- (3) Todos os vértices devem receber as informações uma única vez
		for (int i = 0; i < qtVertex; i++) {
			IloExpr times(env);
			for (int t = 0; t < qtVertex; t++) {
				times += x[t][i];
			}
			KK_MBTProblem.add(times == 1);
		}

		//	- (4) Cada aresta pode ser utilizada uma única vez
		for (int i = 0; i < qtEdges; ++i)	{
			IloExpr used(env);			
			for (int t = 0; t < qtVertex; t++) {
				used += y[t][2*i];
				used += y[t][2*i+1];
			}
			KK_MBTProblem.add( used <= 1 );
		}

		//	- (5) Transmite em cada tempo até C_i vezes
		for (int i = 0; i < qtVertex; i++) {
			for ( int t = 0 ; t < qtVertex; ++t ) {
				IloExpr times(env);
				for ( auto e : graph[i] ){
					times += y[t][ e.idx ];
				}
				KK_MBTProblem.add(times <= capacity[i]);
			}
		}

		// - (5) Transmite a atualização somente se já recebeu
		for (int T = 1; T < qtVertex; T++) {
			for (int e = 0; e < 2*qtEdges; e++) {
				IloExpr can(env);
				for (int t = 0; t < T; t++) {
					can += x[t][ edges[e].first ];
				}
				KK_MBTProblem.add( y[T][e] <= can );
			}
		}

		for (int T = 1; T < qtVertex; ++T) {
			for (int i = 0; i < qtVertex ; ++i) {
				IloExpr enable(env);
				for (int e = 0; e < 2*qtEdges; e++) {
					if( i == edges[e].second ){
						enable += y[T][e];
					}
				}
				KK_MBTProblem.add( x[T][i] == enable );
			}
		}

		//	- (6) 
		for (int t = 0; t < qtVertex; t++) {
			for (int i = 0; i < qtVertex; i++) {
				KK_MBTProblem.add( z >= t * x[t][i]);			
			}
		}
		
		// 	- (1) Objective Function:
		KK_MBTProblem.add(IloMinimize(env, z));

		// -	Show solution:
		cplex.solve();
		int minBroadcast = cplex.getObjValue();

		ofstream file;
		file.open("temp_2.out");
		file << "Minimum broadcast time: " << minBroadcast << endl;
		for (int t = 1; t <= minBroadcast; t++) {
			vector<pair<int,int>> print;
			for (int e = 0; e < 2*qtEdges ; ++e) {
				if( cplex.getValue( y[t][e] ) > 0 ){
					print.push_back(edges[e]);
				}
			}
			file << print.size() << endl;
			for (auto p : print) {
				file << p.first << " " << p.second << endl;
			}
		}

	} catch (const IloException& e) {
		cerr << "Exception caught: " << e << endl;
	}
	catch (...)	{
		cerr << "Unknown exception caught!" << endl;
	}		
	env.end();
	return(0);
}
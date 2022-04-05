#include <bits/stdc++.h>
#include "ArgPack.hpp"
#include "../../MBTLibrary/Graph/WeightedGraph.hpp"
#include "../../MBTLibrary/Graph/WeightedForest.hpp"
// #include "../../MBTLibrary/MBT/BroadcastSchedule.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"
#include "Harutyunyan.hpp"

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

		vector<int> weights_vertices(qtVertices);
		for (int i = 0; i < qtVertices ; ++i) {
			inputFile >> weights_vertices[i];
		}

		WeightedGraph graph = WeightedGraph(qtVertices, weights_vertices);
		WeightedGraph graph2 = WeightedGraph(qtVertices, weights_vertices);

		for (int i = 0; i < qtEdges; ++i) {
			inputFile >> u >> v >> w;
			graph.addEdge(u-1,v-1, w);	
		}

		for (int i = 0; i < qtGateway; ++i) {
			inputFile >> u;
			graph.addSource(u-1);
			graph2.addSource(u-1);
		}
		
		HSCHAGreedy greedy = HSCHAGreedy(&graph);

		WeightedForest forest = greedy.mbt();

		int mbt1 = forest.MBT();
		int lbb1 = graph.LBB();

		const vector<int> dist = graph.getDist();
		vector<tuple<int,int,int>> edges = graph.getEdges();

		for (auto e : edges){
			int u = get<0>(e), v = get<1>(e), w = get<2>(e);

			if( dist[u] > dist[v] ){
				swap(u,v);
			}
			if( dist[u] < dist[v] ){
				if( dist[u] + w + weights_vertices[v] <= mbt1 ){
					graph2.addEdge(u,v,w);
				}
			} else {
				int w2 = min(dist[u] + w + weights_vertices[v], dist[v] + w + weights_vertices[u]);
				if( w2 <= mbt1 ){
					graph2.addEdge(u,v,w);
				}
			}
		}

		HSCHAGreedy greedy2 = HSCHAGreedy(&graph2);
		WeightedForest forest2 = greedy2.mbt();

		int lbb2 = graph2.LBB();
		int mbt2 = forest2.MBT();

		if( args.writeFile ){
			outputFile.open(args.outputFile, ios::out);
		}

		if( args.printOutput ){
			cout << "LBB_O: " << lbb1 << endl;
			cout << "MBT_O: " << mbt1 << endl;
			cout << "R%: " << 1 - ((1.0 * graph2.numberEdges()) / (1.0 * graph.numberEdges())) << endl;
			cout << "LBB_R: " << lbb2 << endl;
			cout << "MBT_R: " << mbt2 << endl;
			cout << "Graph: " << qtVertices << " " << qtGateway << " " << graph2.numberEdges() << endl;
			for (int i = 0; i < qtVertices ; ++i) {
				if( i ) cout << " ";
				cout << weights_vertices[i];
			}
			cout << endl;
			vector<tuple<int,int,int>> edges2 = graph2.getEdges();
			for (auto e : edges2){
				int u = get<0>(e), v = get<1>(e), w = get<2>(e);
				cout << u+1 << " " << v+1 << " " << w << endl;
			}
		}

		if( args.writeFile ){
			outputFile << "LBB_O: " << lbb1 << endl;
			outputFile << "MBT_O: " << mbt1 << endl;
			outputFile << "R%: " << 1 - ((1.0 * graph2.numberEdges()) / (1.0 * graph.numberEdges())) << endl;
			outputFile << "LBB_R: " << lbb2 << endl;
			outputFile << "MBT_R: " << mbt2 << endl;
			outputFile << "Graph: " << qtVertices << " " << qtGateway << " " << graph2.numberEdges() << endl;
			for (int i = 0; i < qtVertices ; ++i) {
				if( i ) outputFile << " ";
				outputFile << weights_vertices[i];
			}
			outputFile << endl;
			vector<tuple<int,int,int>> edges2 = graph2.getEdges();
			for (auto e : edges2){
				int u = get<0>(e), v = get<1>(e), w = get<2>(e);
				outputFile << u+1 << " " << v+1 << " " << w << endl;
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
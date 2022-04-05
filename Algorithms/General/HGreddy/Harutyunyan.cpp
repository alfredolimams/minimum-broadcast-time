#include "Harutyunyan.hpp"

using namespace std;

HarutyunyanGreedy::HarutyunyanGreedy(WeightedGraph * graph){
	this->graph = graph;
}

HarutyunyanGreedy::~HarutyunyanGreedy(){

}

int HarutyunyanGreedy::mbt(){
	int n = graph->numberVertices();
	vector<int> dist(n, INT_MAX);

	vector<int> sources = graph->getSources();
	vector<int> weights = graph->getWeights();

	vector<int> informed;
	vector<bool> uninformed(n, true);
	for(auto s : sources){
		informed.push_back(s);
		uninformed[s] = false;
		dist[s] = weights[s];
	}

	int u = 0, v = 0, w = 0;
	int broadcastTime = 0;
	while( (int)(informed.size()) < n ){
		int minValue = INT_MAX;

		// Get min edge
		for (auto i : informed) {
			for(auto j : graph->getNeighbors(i)){
				if( !uninformed[j.first] ) continue;
				if( dist[i] + j.second + weights[j.first] < minValue ){
					minValue = dist[i] + j.second + weights[j.first];
					u = i;
					v = j.first;
					w = j.second;
				}
			}
		}
		dist[v] = dist[u] + weights[v] + w;
		broadcastTime = max(broadcastTime, dist[v]);
		dist[u] += w;
		informed.push_back(v);
		uninformed[v] = false; 
	}

	return broadcastTime;
}
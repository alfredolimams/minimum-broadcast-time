#include "Grasp.hpp"

using namespace std;

GRASP::GRASP(WeightedGraph * graph, MTRand * rand, BossaTimer * timer){
	this->graph = graph;
	this->rand = rand;
	this->timer = timer;
	this->lb = this->graph->LBB();
}

GRASP::~GRASP(){

}

WeightedForest GRASP::mbt(double alpha){
	int qtV = graph->numberVertices();
	int qtE = graph->numberEdges();
	auto edges = graph->getEdges();

	vector<int> dist(qtV, 1<<20);
	vector<int> sources = graph->getSources();
	int qtS = sources.size();

	vector<int> weights = graph->getWeights();

	WeightedForest forest = WeightedForest(qtV, weights);

	vector<int> informed;
	informed.reserve(qtV);

	vector<bool> uninformed(qtV, true);
	for(auto s : sources){
		informed.push_back(s);
		uninformed[s] = false;
		dist[s] = weights[s];
		forest.addRoot(s);
	}

	int u, v, w, s, l, e, value;
	while( forest.numberEdges() + qtS < qtV ){
		int minValue = 1<<20;
		int maxValue = 0;

		vector< pair<int,int> > RCL;
		// cout << "Possible edges: " << endl;

		for (int i = 0; i < qtE; ++i) {
			u = get<0>(edges[i]);
			v = get<1>(edges[i]);
			w = get<2>(edges[i]);
			
			if( uninformed[v] == uninformed[u] ) continue;

			if( !uninformed[v] )
				swap(u, v);
			
			s = dist[u] + weights[v] + w;
			minValue = min(minValue, s);
			maxValue = max(maxValue, s);

			RCL.push_back({s, i});
			// cout << u << " - " << v << " : " << s << endl;
		}

		sort(RCL.begin(), RCL.end());


		value = minValue + (maxValue - minValue) * alpha;

		//Binary search to find limit - otimizar
		l = RCL.size();
		for (int i = 0; i < l; ++i) {
			if( RCL[i].first > value ){
				l = i;
				break;
			}
		}

		//Choosen a random edge
		e = this->rand->randInt(l-1);

		u = get<0>(edges[RCL[e].second]);
		v = get<1>(edges[RCL[e].second]);
		w = get<2>(edges[RCL[e].second]);

		if( !uninformed[v] )
			swap(u, v);

		dist[v] = dist[u] + weights[v] + w;
		forest.addEdge(u, v, w);
		dist[u] += w;
		informed.push_back(v);
		uninformed[v] = false;

	}

	return forest;
}

WeightedForest GRASP::compute(unsigned T, double timeout, double alpha){

	this->timer->reset();
	this->timer->start();
	WeightedForest best = this->mbt(0);
	ttt = this->timer->getTime();
	if( timeout > 0 ){
		this->timer->setMaxTime(timeout);
	}

	unsigned t = 0;
	while( true ){
		if( T ){
			t++;
		}

		WeightedForest solution_i = this->mbt(alpha);
		if( solution_i.MBT() < best.MBT() ){
			best = solution_i;
			ttt = this->timer->getTime();
		}

		if( this->stop(best.MBT(), t, T) ){
			break;
		}
	}

	return best;
}

bool GRASP::stop(unsigned v, unsigned t, unsigned T){
	if( this->timer->isTimeExpired() || ( T && t >= T) || this->lb == v ){
		return true;
	}
	return false;
}

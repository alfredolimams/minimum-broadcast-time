/*
 * MBTDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "MBTDecoder.hpp"

using namespace std;

#define DEBUG if(0)

int bestAnswer;

MBTDecoder::MBTDecoder() {
}

MBTDecoder::~MBTDecoder() {}

bool MBTDecoder::init(Graph * graph, BossaTimer * timer){
	this->graph = graph;
	this->timer = timer;
	bestAnswer = 1<<20;
	this->lb = graph->LBB_BFS();
	DEBUG cout << "LBB-BFS " << this->lb << endl;
	return true;
}

void MBTDecoder::setBestAnswer(int value) const{
	if( value < bestAnswer ){
		bestAnswer = value;
		this->timer->lap();
	}
}

int MBTDecoder::getBestAnswer() {
	return bestAnswer;
}

bool MBTDecoder::stop() const {
	bool can_stop = false;
	// can_stop |= (bestAnswer == this->lb);
	// can_stop |= ((bestAnswer != 1<<20) and (this->timer->isTimeExpired()));
	return can_stop;
}

// Runs in \Theta(n \log n):
double MBTDecoder::decode(const std::vector< double >& chromosome) const {
	
	if (this->stop()) {
		return chromosome.size();
	}
	
	// O(E * lg(E))
	int qt_vertices = this->graph->numberVertices();
	vector<set<pair<double,int>>> ordered_graph(qt_vertices);
	for (int i = 0; i < qt_vertices; ++i) {
		for (auto v : this->graph->getNeighbors(i) ) {
			ordered_graph[i].insert({chromosome[v],v});
		}
	}

	bool found = false;
	int sz = chromosome.size();
	set<pair<double,unsigned>> ranking;
	set<unsigned> transmitters;

	int fit = 0;
	// O(V)
	for ( auto v : this->graph->getGateways() ){
		ranking.insert( {chromosome[v], v} );
		transmitters.insert(v);
	}

	// O(V*E)
	while( !found && !this->stop() ){
		set<int> new_transmitters; // Store new transmitters

		// Process of broadcast
		for ( auto v : ranking ){
			vector<pair<double,unsigned>> order;
			auto it = ordered_graph[v.second].begin();
			while( it != ordered_graph[v.second].end() ){
				if( transmitters.count(it->second) or new_transmitters.count(it->second) ){
					it = ordered_graph[v.second].erase(it);
					continue;
				}
				new_transmitters.insert(it->second);
				break;
			}
		}

		// Update transmitters
		for ( auto u : new_transmitters ){
			ranking.insert({chromosome[u],u});
			transmitters.insert(u);
		}
		fit++;
		// Check if all receveid the message.
		if (int(transmitters.size()) == sz) found = true;
	}

	if( found ){
		setBestAnswer(fit);
		return fit;
	}
	return chromosome.size();
}
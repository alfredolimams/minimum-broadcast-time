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
	can_stop |= (bestAnswer == this->lb);
	can_stop |= ((bestAnswer != 1<<20) and (this->timer->isTimeExpired()));
	return can_stop;
}

// Runs in \Theta(n \log n):
double MBTDecoder::decode(const std::vector< double >& chromosome) const {
	
	if (this->stop()) {
		return chromosome.size();
	}
	bool found = false;
	int sz = chromosome.size();
	Forest forest = Forest(sz);
	set<pair<double,unsigned>> ranking;
	set<unsigned> transmitters;

	// O(V)
	for ( auto v : this->graph->getGateways() ){
		ranking.insert( {chromosome[v], v} );
		forest.addRoot(v);
		transmitters.insert(v);
	}

	// O(V*E)
	while( !found && !this->stop() ){
		set<int> new_transmitters; // Store new transmitters

		// Process of broadcast
		for ( auto v : ranking ){
			vector<pair<double,unsigned>> order;

			for ( auto u : this->graph->getNeighbors(v.second) ){
				if( transmitters.count(u) or new_transmitters.count(u) ){
					continue;
				}
				order.push_back({chromosome[u],u});
			}

			if( int(order.size()) ){
				pair<double,unsigned> item = 
					*min_element(order.begin(),order.end());
				new_transmitters.insert(item.second);
				forest.addEdge(v.second,item.second);
			}
		}

		// Update transmitters
		for ( auto u : new_transmitters ){
			ranking.insert({chromosome[u],u});
			transmitters.insert(u);
		}
		// Check if all receveid the message.
		if (int(transmitters.size()) == sz) found = true;
	}
	int fit = forest.MBT();
	if( found ){
		setBestAnswer(fit);
		return fit;
	}
	return chromosome.size();
}
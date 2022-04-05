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
	bool found = true;
	int qtVertices = chromosome.size();
	Forest forest = Forest(qtVertices);
	vector<pair<double,unsigned>> ranking;

	for (int v = 0; v < qtVertices ; ++v) {
		if ( !this->graph->getGateways().count(v) ){
			ranking.push_back( {chromosome[v], v} );
		} else{
			forest.addRoot(v);
		}
	}

	sort(ranking.begin(),ranking.end());
	for (int r = 0; r < int(ranking.size()) ; ++r) {
		if( this->stop() ){
			found = false;
			break;
		}
		int v = ranking[r].second;
		set<int> paths = this->graph->getFathers(v);
		int u = *paths.begin();
		for (auto i = paths.begin() ; i != paths.end() ; ++i){
		 	if( chromosome[*i] < chromosome[u] ){
		 		u = *i;
		 	}
		}
		forest.addEdge(v,u);
	}

	if( found ){
		int fit = forest.MBT();
		setBestAnswer(fit);
		return fit;
	}
	return chromosome.size();
}
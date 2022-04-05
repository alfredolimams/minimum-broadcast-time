/*
 * Decoder_BPI0.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "Decoder_BPI0.hpp"

using namespace std;

Decoder_BPI0::Decoder_BPI0(string name){
	ifstream inputFile;
	inputFile.open(name, ios::in);
	inputFile >> pe;
	inputFile >> pm;
	inputFile >> rhoe;
	inputFile.close();
}

Decoder_BPI0::Decoder_BPI0(double pe, double pm, double rhoe){
	this->pe = pe;
	this->pm = pm;
	this->rhoe = rhoe;
}

Decoder_BPI0::~Decoder_BPI0(){
	delete this;
}

// Runs in \Theta(n \log n):
double Decoder_BPI0::decode(const std::vector< double >& chromosome) const {
	
	if (this->stop()) {
		return chromosome.size();
	}

	Forest broadcast = this->build_forest(chromosome);
	int mbt = broadcast.MBT();
	setBestAnswer(mbt);
	return mbt;

}

// Runs in \Theta(n \log n):
Forest Decoder_BPI0::build_forest(const std::vector< double >& chromosome) const {

	int sz = chromosome.size();
	int qt_transmitters = this->graph->getSources().size();
	vector<pair<double,unsigned>> ranking;
	ranking.reserve(sz);
	vector<bool> transmitters(sz, false);
	
	Forest f = Forest(sz);
	// O(V)
	for ( auto v : this->graph->getSources() ){
		ranking.push_back( {chromosome[v], v} );
		transmitters[v] = true;
		f.addRoot(v);
	}

	// O(V*E)
	while( qt_transmitters < sz ){

		vector<int> new_transmitters; // Store new transmitters
		new_transmitters.reserve(sz/2);
	
		// Process of broadcast
		sort(ranking.begin(),ranking.end());
		for ( auto v : ranking ){
			int idxBest = -1;
			for ( auto u : this->graph->getNeighbors(v.second) ){
				if( transmitters[u] ){
					continue;
				}
				if( idxBest == -1 ){
					idxBest = u;
				} else if( chromosome[idxBest] > chromosome[u] ){
					idxBest = u;
				}
			}
			if( idxBest != -1 ){
				transmitters[idxBest] = true;
				new_transmitters.push_back(idxBest);
				f.addEdge(v.second, idxBest);
			}
		}

		// Update transmitters
		for ( auto u : new_transmitters ){
			ranking.push_back({chromosome[u],u});
			qt_transmitters++;
		}
	}

	return f;
}

BroadcastSchedule const Decoder_BPI0::broadcast_schedule(const std::vector< double >& chromosome) const{
	Forest forest = this->build_forest(chromosome);
	return BroadcastSchedule(forest.getRoots(), forest.getOrderingTransmissions(), forest.MBT());
}

vector<double> const Decoder_BPI0::broadcast_to_cromossome(BroadcastSchedule const schedule) const{

	unsigned sz = graph->numberVertices();
	double stepper = 1.0/sz;
	int count = 0;
	vector<double> chromosome(sz);	

	// Initial transmitters list
	for ( auto s : graph->getSources() ) {
		chromosome[s] = stepper * count;
		count++;
	}	

	for ( auto broadcasts : schedule.getSchedule() ) {
		for ( auto broadcast : broadcasts.second ){
			chromosome[ broadcast.second ] = stepper * count;
			count++;
		}
	}
	
	return chromosome;
}
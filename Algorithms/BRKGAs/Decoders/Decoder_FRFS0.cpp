/*
 * Decoder_FRFS0.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "Decoder_FRFS0.hpp"

using namespace std;

Decoder_FRFS0::Decoder_FRFS0(string name){
	ifstream inputFile;
	inputFile.open(name, ios::in);
	inputFile >> pe;
	inputFile >> pm;
	inputFile >> rhoe;
	inputFile.close();
}

Decoder_FRFS0::Decoder_FRFS0(double pe, double pm, double rhoe){
	this->pe = pe;
	this->pm = pm;
	this->rhoe = rhoe;
}

Decoder_FRFS0::~Decoder_FRFS0(){
	delete this;
}

// Runs in \Theta(n \log n):
double Decoder_FRFS0::decode(const std::vector< double >& chromosome) const {
	
	if (this->stop()) {
		return chromosome.size();
	}

	int sz = chromosome.size();
	int qt_transmitters = this->graph->getSources().size();
	vector<pair<double,unsigned>> ranking;
	vector<bool> transmitters(sz, false);
	
	int fit = 0;
	// O(V)
	for ( auto v : this->graph->getSources() ){
		ranking.push_back( {chromosome[v], v} );
		transmitters[v] = true;
	}

	// O(V*E)
	sort(ranking.begin(),ranking.end());
	while( qt_transmitters < sz ){

		vector<int> new_transmitters; // Store new transmitters
		new_transmitters.reserve(sz/2);
	
		// Process of broadcast
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
			}
		}

		// Update transmitters
		for ( auto u : new_transmitters ){
			ranking.push_back({chromosome[u],u});
			qt_transmitters++;
		}
		fit++;
	}

	setBestAnswer(fit);
	return fit;
}

BroadcastSchedule const Decoder_FRFS0::broadcast_schedule(const std::vector< double >& chromosome) const {
	int sz = chromosome.size();
	int qt_transmitters = this->graph->getSources().size();
	vector<pair<double,unsigned>> ranking;
	vector<bool> transmitters(sz, false);
	vector<vector<int>> ordering(sz);
	int mbt = 0;
	// O(V)
	for ( auto v : this->graph->getSources() ){
		ranking.push_back( {chromosome[v], v} );
		transmitters[v] = true;
	}

	// O(V*E)
	sort(ranking.begin(),ranking.end());
	while( qt_transmitters < sz ){
		mbt++;
		vector<int> new_transmitters; // Store new transmitters
		new_transmitters.reserve(sz/2);
	
		// Process of broadcast
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
				ordering[v.second].push_back(idxBest);
			}
		}

		// Update transmitters
		for ( auto u : new_transmitters ){
			ranking.push_back({chromosome[u],u});
			qt_transmitters++;
		}
	}

	return BroadcastSchedule(this->graph->getSources(), ordering, mbt);
}

vector<double> const Decoder_FRFS0::broadcast_to_cromossome(BroadcastSchedule const schedule) const{

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
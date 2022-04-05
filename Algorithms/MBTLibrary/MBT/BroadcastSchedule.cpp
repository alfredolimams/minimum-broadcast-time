#include "BroadcastSchedule.hpp"

BroadcastSchedule::BroadcastSchedule(){
	this->mbt = INT_MAX;
}

BroadcastSchedule::BroadcastSchedule(const vector<int> & sources,const vector<vector<int>> & ordering, int mbt){

	this->mbt = mbt;
	unsigned sz = ordering.size();	
	if ( sz < sources.size() ){
		return;
	}
	vector<int> transmitters;
	vector<unsigned> transmition(sz, 0);
	
	// Initial transmitters list
	for ( auto s : sources ) {
		transmitters.push_back(s);
	}

	// Schedule Broadcast
	int time = 0;
	while( transmitters.size() < sz ){
		// vector<pair<int,int>> transmitions;
		vector<int> received;
		// Transmition
		for ( auto t : transmitters ) {
			if( transmition[t] < ordering[t].size() ){
				received.push_back(ordering[t][transmition[t]]);
				// transmitions.push_back({t,ordering[t][transmition[t]]});
				this->schedule[time].push_back({t,ordering[t][transmition[t]]});
				transmition[t]++;
			}
		}
		time++;
		// this->schedule.push_back(transmitions);
		// Update transmitters list
		for ( auto r : received ) {
			transmitters.push_back(r);
		}
	}
}

int const BroadcastSchedule::getMBT(){
	return this->mbt;
}

map<int,vector<pair<int,int>>> const BroadcastSchedule::getSchedule() const{
	return this->schedule;
}

WeightedForest const BroadcastSchedule::getForest() const{
	return this->solution;
}

BroadcastSchedule::BroadcastSchedule(const WeightedForest & forestBroadcast){
	this->solution = forestBroadcast;
	this->mbt = forestBroadcast.MBT();

	const vector<vector<int>> ordering = forestBroadcast.getOrderingTransmissions();
	const vector<int> sources = forestBroadcast.getRoots(); 
	unsigned sz = ordering.size();	

	const vector<tuple<int,int,int>> edgesWeights = forestBroadcast.getEdges();
	const vector<int> vertexWeights = forestBroadcast.getWeights();

	vector<int> timers(sz, INT_MAX);
	vector<int> transmitters;
	vector<unsigned> transmition(sz, 0);
	
	// Initial transmitters list
	for ( auto s : sources ) {
		transmitters.push_back(s);
		timers[s] = vertexWeights[s];
	}

	// Schedule Broadcast
	while( transmitters.size() < sz ){
		vector<int> received;
		// Transmition
		for ( auto t : transmitters ) {
			if( transmition[t] < ordering[t].size() ){
				int r = ordering[t][transmition[t]];
				received.push_back(r);
				transmition[t]++;
				this->schedule[ timers[t] ].push_back({t,r});
				int cost = 0;
				for ( auto e : edgesWeights ) {
					int u = get<0>(e);
					int v = get<1>(e);
					if( min(t,r) == min(u,v) && max(t,r) == max(u,v) ){
						cost = get<2>(e);
						break;
					}
				}
				timers[t] += cost;
				timers[r] = timers[t] + vertexWeights[r];
			}
		}
		// Update transmitters list
		for ( auto r : received ) {
			transmitters.push_back(r);
		}
	}
}
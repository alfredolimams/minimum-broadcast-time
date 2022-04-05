/*
 * WeightedForest.cpp
 *
 *  Created on: Dec 20th, 2020
 *      Author: Alfredo
 */

#include "WeightedForest.hpp"
#include <limits.h>
#include <math.h>
#include <queue>
#include<tuple>
#include <iostream>

using namespace std;

WeightedForest::WeightedForest() {
	this->mbt = INT_MAX;
}

WeightedForest::WeightedForest(int qtNodes, vector<int> &weights) {
	this->mbt = INT_MAX;
	this->adj_list.resize(qtNodes);
	this->vstd.resize(qtNodes, 0);
	this->ordering.resize(qtNodes);
	this->weights = weights;
	this->mbts.resize(qtNodes, INT_MAX);
}

WeightedForest::~WeightedForest() {
	this->adj_list.clear();
	this->vstd.clear();
	this->ordering.clear();
	this->edges.clear();
	this->weights.clear();
	this->mbts.clear();
}

bool WeightedForest::addEdge(int u, int v, int w) {
	int sz = this->adj_list.size();

	if( u < 0 or v < 0 ){
		return false;
	} else if( u >= sz or v >= sz ){
		return false;
	}

	this->adj_list[u].push_back({v,w});
	this->adj_list[v].push_back({u,w});
	this->edges.push_back({min(u,v),max(u,v),w});
	return true;
}

vector<pair<int,int>> const WeightedForest::getNeighbors(int v) const {
	return this->adj_list[v];
}

bool WeightedForest::addRoot(int u){
	int sz = this->adj_list.size();
	if( u < 0 or u >= sz ){
		return false;
	}
	this->roots.push_back(u);
	return true;
}

unsigned int const WeightedForest::scheduleBroadcast(int v) const{
	if ( vstd[v] ){
		return this->mbts[v];
	}
	vstd[v] = true;
	int time = 0;
	priority_queue<tuple<int,int,int>> pq;
	for (unsigned i = 0; i < this->adj_list[v].size() ; ++i) {
		auto u = adj_list[v][i];
		if( vstd[ u.first ] ) continue;
		pq.push( {scheduleBroadcast(u.first), u.second, u.first} );
	}
	int t = 0;
	while( !pq.empty() ){
		auto value = pq.top(); pq.pop();
		int b = get<0>(value);
		int w = get<1>(value);
		int u = get<2>(value);
		t += w;
		time = max(time, b + t);
		ordering[v].push_back(u);
	}
	this->mbts[v] = time + this->weights[v];
	return this->mbts[v];
}

unsigned int const WeightedForest::MBT() const{

	if( !this->adj_list.size() ){
		return INT_MAX;
	}

	if( this->numberEdges() + this->numberRoots() != this->numberVertices()){
		return INT_MAX;
	}

	if (this->mbt != INT_MAX){
		return this->mbt;
	}

	this->mbt = 0;
	for (auto r : this->roots){
		this->mbt = max(this->mbt, this->scheduleBroadcast(r));
	}

	return this->mbt;
}

int const WeightedForest::numberVertices() const{
	return this->adj_list.size();
}

int const WeightedForest::numberEdges() const{
	return this->edges.size();
}

int const WeightedForest::numberRoots() const{
	return this->roots.size();
}


vector<tuple<int,int,int>> const WeightedForest::getEdges() const{
	return this->edges;
}

vector<vector<int>> const WeightedForest::getOrderingTransmissions() const {
	this->MBT();
	return this->ordering;	
}

vector<int> const WeightedForest::getRoots() const{
	return this->roots;
}

vector<int> const WeightedForest::getWeights() const{
	return this->weights;
}
/*
 * Forest.cpp
 *
 *  Created on: Aug 14, 2020
 *      Author: Alfredo
 */

#include "Forest.hpp"
#include <limits.h>
#include <math.h>
#include <queue>
#include <iostream>

using namespace std;

Forest::Forest() {
}

Forest::Forest(int qtNodes) {
	this->mbt = -1;
	this->adj_list.resize(qtNodes);
	this->vstd.resize(qtNodes, 0);
	this->ordering.resize(qtNodes);
}

Forest::~Forest() {
	this->adj_list.clear();
	this->vstd.clear();
	this->ordering.clear();
	this->edges.clear();
}

bool Forest::addEdge(int u, int v) {
	int sz = this->adj_list.size();

	if( u < 0 or v < 0 ){
		return false;
	} else if( u >= sz or v >= sz ){
		return false;
	}

	this->adj_list[u].push_back(v);
	this->adj_list[v].push_back(u);
	this->edges.push_back({u,v});
	return true;
}

vector<int> const Forest::getNeighbors(int v) const {
	return this->adj_list[v];
}

bool Forest::addRoot(int u){
	int sz = this->adj_list.size();
	if( u < 0 or u >= sz ){
		return false;
	}
	this->roots.push_back(u);
	return true;
}

int const Forest::scheduleBroadcast(int v) const{
	vstd[v] = true;
	int time = 0;
	priority_queue<pair<int,int>> pq;
	for (auto u : this->adj_list[v]) {
		if( vstd[u] ) continue;
		pq.push( {scheduleBroadcast(u),u} );
	}
	int t = 1;
	while( !pq.empty() ){
		auto value = pq.top(); pq.pop();
		time = max(time, value.first + t);
		ordering[v].push_back(value.second);
		t++;
	}
	return time;
}

int const Forest::MBT() const{

	if (this->mbt != -1){
		return this->mbt;
	}

	this->mbt = 0;
	for (auto r : this->roots){
		this->mbt = max(this->mbt, this->scheduleBroadcast(r));
	}

	return this->mbt;
}

int Forest::numberVertices(){
	return this->adj_list.size();
}

vector<pair<int,int>> const Forest::getEdges() const{
	return this->edges;
}

vector<vector<int>> const Forest::getOrderingTransmissions() const {
	this->MBT();
	return this->ordering;	
}

vector<int> const Forest::getRoots() const{
	return this->roots;
}
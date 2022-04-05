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
	this->adjList.resize(qtNodes);
	this->vstd.resize(qtNodes, 0);
}

Forest::~Forest() {
}

bool Forest::addEdge(int u, int v) {
	int sz = this->adjList.size();

	if( u < 0 or v < 0 ){
		return false;
	} else if( u >= sz or v >= sz ){
		return false;
	}

	this->adjList[u].insert(v);
	this->adjList[v].insert(u);
	return true;
}

set<int> const Forest::getNeighbors(int v) const {
	return this->adjList[v];
}

bool Forest::addRoot(int u){
	int sz = this->adjList.size();
	if( u < 0 or u >= sz ){
		return false;
	}
	this->roots.insert(u);
	return true;
}

int Forest::scheduleBroadcast(int v){
	vstd[v] = true;
	int time = 0;
	priority_queue<pair<int,int>> pq;
	for (auto u : this->adjList[v]) {
		if( vstd[u] ) continue;
		pq.push( {scheduleBroadcast(u),u} );
	}
	int t = 1;
	while( !pq.empty() ){
		auto value = pq.top(); pq.pop();
		time = max(time, value.first + t);
		t++;
	}
	return time;
}

int Forest::MBT(){
	int value = 0;
	for (auto r : this->roots){
		value = max(value, this->scheduleBroadcast(r));
	}
	return value;
}

int Forest::numberVertices(){
	return this->adjList.size();
}

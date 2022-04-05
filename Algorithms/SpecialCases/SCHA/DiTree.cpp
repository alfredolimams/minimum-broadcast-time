/*
 * DiTree.cpp
 *
 *  Created on: Aug 14, 2020
 *      Author: Alfredo
 */

#include "DiTree.hpp"
#include <limits.h>
#include <math.h>
#include <queue>
#include <iostream>

using namespace std;

DiTree::DiTree() {
}

DiTree::DiTree(int qtNodes) {
	this->root = 0;
	this->adjList.resize(qtNodes);
	this->vstd.resize(qtNodes, 0);
}

DiTree::~DiTree() {
}

bool DiTree::addEdge(int u, int v) {
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

set<int> const DiTree::getNeighbors(int v) const {
	return this->adjList[v];
}

bool DiTree::setRoot(int u){
	int sz = this->adjList.size();
	if( u < 0 or u >= sz ){
		return false;
	}
	this->root = u;
	return true;
}

int DiTree::scheduleBroadcast(int v){
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

int DiTree::MBT(){
	return this->scheduleBroadcast(this->root);
}

int DiTree::numberVertices(){
	return this->adjList.size();
}

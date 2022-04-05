/*
 * Graph.cpp
 *
 *  Created on: Jan 27, 2020
 *      Author: Alfredo
 */

#include "Graph.hpp"
#include <limits.h>
#include <math.h>
#include <queue>
#include <iostream>

using namespace std;

Graph::Graph() {

}

Graph::Graph(int qtNodes) { 
	this->adjList.resize(qtNodes);
	this->fathers.resize(qtNodes);
}

Graph::~Graph() {
}

set<int> const Graph::getFathers(int v) const {
	return this->fathers[v];
}


set<int> const Graph::getGateways() const {
	return this->gateways;
}


bool Graph::addEdge(int u, int v) {
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

set<int> const Graph::getNeighbors(int v) const {
	return this->adjList[v];
}


bool Graph::addGateway(int u){
	int sz = this->adjList.size();
	if( u < 0 or u >= sz ){
		return false;
	}
	gateways.insert(u);
	return true;
}

int Graph::LBB_BFS(){
	int qt_vertices = this->adjList.size();
	int qt_gateways = this->gateways.size();
	vector<int> dist(qt_vertices, 1<<20);
	queue<int> q;
	int maxDist = ceil(log2(qt_vertices/qt_gateways));
	
	for( auto s : this->gateways ){
		dist[s] = 0;
		q.push(s);
	}
	
	while( !q.empty() ) {
		int front = q.front(); q.pop();
		for( int v : this->adjList[front] ){
			if( dist[front]+1 < dist[v] ) {
				dist[v] = dist[front]+1;
				this->fathers[v].insert(front);
				q.push(v);
				maxDist = max(maxDist, dist[v]);
			} else if( dist[front]+1 == dist[v] ){
				this->fathers[v].insert(front);
			}
		}
	}

	cout << "LBB-BFS: " << maxDist << endl;
	return maxDist;
}

int Graph::numberVertices(){
	return this->adjList.size();
}

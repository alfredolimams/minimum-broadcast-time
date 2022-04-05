/*
 * UnweightedGraph.cpp
 *
 *  Created on: Jan 27, 2020
 *      Author: Alfredo
 */

#include "UnweightedGraph.hpp"
#include <limits.h>
#include <math.h>
#include <queue>
#include <iostream>

using namespace std;

UnweightedGraph::UnweightedGraph() {

}

UnweightedGraph::UnweightedGraph(int qtNodes) { 
	this->adjList.resize(qtNodes);
	this->degree.resize(qtNodes);
	this->dist.resize(qtNodes);
}

UnweightedGraph::~UnweightedGraph() {

}

vector<int> const UnweightedGraph::getSources() const {
	return this->sources;
}


bool UnweightedGraph::addEdge(int u, int v) {
	int sz = this->adjList.size();

	if( u < 0 or v < 0 ){
		return false;
	} else if( u >= sz or v >= sz ){
		return false;
	}
	this->degree[u]++;
	this->degree[v]++;
	
	this->adjList[u].push_back(v);
	this->adjList[v].push_back(u);

	this->edges.push_back({min(u,v),max(u,v)});
	return true;
}

vector<int> const UnweightedGraph::getNeighbors(int v) const {
	return this->adjList[v];
}


bool UnweightedGraph::addSource(int u){
	int sz = this->adjList.size();
	if( u < 0 or u >= sz ){
		return false;
	}
	sources.push_back(u);
	return true;
}

int UnweightedGraph::LBB_BFS() const{
	int qt_vertices = this->adjList.size();
	int qt_sources = this->sources.size();
	vector<int> dist(qt_vertices, 1<<20);
	queue<int> q;
	int maxDist = ceil(log2(qt_vertices/qt_sources));
	
	for( auto s : this->sources ){
		dist[s] = 0;
		q.push(s);
	}
	
	while( !q.empty() ) {
		int front = q.front(); q.pop();
		for( auto v : this->adjList[front] ){
			if( dist[v] == 1<<20 ) {
				dist[v] = dist[front]+1;
				q.push(v);
				maxDist = max(maxDist, dist[v]);
			}
		}
	}
	return maxDist;
}

int UnweightedGraph::numberVertices(){
	return this->adjList.size();
}

int UnweightedGraph::numberEdges(){
	return this->edges.size();
}

vector<int> const & UnweightedGraph::getDist() const{
	return this->dist;
}

vector<int> const & UnweightedGraph::getDegree() const{
	return this->degree;
}

vector<pair<int,int>> const & UnweightedGraph::getEdges() const{
	return this->edges;
}

double UnweightedGraph::getDensity(){
	double d = 2 * this->numberEdges();
	int qt_v = this->numberVertices();
	return d / (qt_v * (qt_v-1) );
}

void UnweightedGraph::sortEdges(){
	sort(this->edges.begin(), this->edges.end());
}

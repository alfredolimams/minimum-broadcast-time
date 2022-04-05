/*
 * WeightedGraph.cpp
 *
 *  Created on: Dec 19th, 2020
 *      Author: Alfredo
 */

#include "WeightedGraph.hpp"
#include <limits.h>
#include <math.h>
#include <queue>
#include <iostream>

using namespace std;

WeightedGraph::WeightedGraph() {

}

WeightedGraph::WeightedGraph(int qtNodes, vector<int> & weights) { 
	this->adjList.resize(qtNodes);
	this->degree.resize(qtNodes);
	this->dist.resize(qtNodes);
	this->weights = weights;
}

WeightedGraph::~WeightedGraph() {

}

vector<int> const WeightedGraph::getSources() const {
	return this->sources;
}


bool WeightedGraph::addEdge(int u, int v, int w) {
	int sz = this->adjList.size();

	if( u < 0 or v < 0 ){
		return false;
	} else if( u >= sz or v >= sz ){
		return false;
	} else if( w < 0 ){
		return false;
	}
	this->degree[u]++;
	this->degree[v]++;
	
	this->adjList[u].push_back({v,w});
	this->adjList[v].push_back({u,w});

	this->edges.push_back({min(u,v),max(u,v),w});
	return true;
}

vector<pair<int,int>> const WeightedGraph::getNeighbors(int v) const {
	return this->adjList[v];
}


bool WeightedGraph::addSource(int u){
	int sz = this->adjList.size();
	if( u < 0 or u >= sz ){
		return false;
	}
	sources.push_back(u);
	return true;
}

int WeightedGraph::LBB() const{
	int qt_vertices = this->adjList.size();
	// int qt_sources = this->sources.size();
	for (int i = 0; i < qt_vertices ; ++i) {
		this->dist[i] = INT_MAX;
	}
	priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
	int sumWeights = 0;
	
	for( auto s : this->sources ){
		this->dist[s] = this->weights[s];
		sumWeights += this->weights[s];
		pq.push({this->weights[s], s});
	}
	
	int maxDist = 0;
	
	while( !pq.empty() ) {
		auto front = pq.top(); pq.pop();
		int d = front.first, u = front.second;
		
		if (d > dist[u])
			continue;

		for( auto v : this->adjList[u] ) {
			if( this->dist[u] + this->weights[v.first] + v.second < this->dist[v.first] ) {
				this->dist[v.first] = this->dist[u] + this->weights[v.first] + v.second;
				pq.push( { this->dist[v.first], v.first } );
			}
		}
	}

	for (int i = 0; i < qt_vertices ; ++i) {
		maxDist = max(maxDist, this->dist[i]);
	}

	return maxDist;
}

int WeightedGraph::numberVertices(){
	return this->adjList.size();
}

int WeightedGraph::numberEdges(){
	return this->edges.size();
}

vector<int> const & WeightedGraph::getDist() const{
	return this->dist;
}

vector<int> const & WeightedGraph::getWeights() const{
	return this->weights;
}

vector<int> const & WeightedGraph::getDegree() const{
	return this->degree;
}

vector<tuple<int,int,int>> const & WeightedGraph::getEdges() const{
	return this->edges;
}

double WeightedGraph::getDensity(){
	double d = 2 * this->numberEdges();
	int qt_v = this->numberVertices();
	return d / (qt_v * (qt_v-1) );
}

WeightedForest WeightedGraph::HSCHAGreedy(){
	int n = this->numberVertices();
	vector<int> distL(n, INT_MAX);
	vector<int> sources = this->getSources();
	vector<int> weights = this->getWeights();

	WeightedForest forest = WeightedForest(n, weights);

	vector<int> informed;
	vector<bool> uninformed(n, true);
	for(auto s : sources){
		informed.push_back(s);
		uninformed[s] = false;
		distL[s] = weights[s];
		forest.addRoot(s);
	}

	int u = 0, v = 0, w = 0;
	while( (int)(informed.size()) < n ){
		int minValue = INT_MAX;

		// Get min edge
		for (auto i : informed) {
			for(auto j : this->getNeighbors(i)){
				if( !uninformed[j.first] ) continue;
				if( distL[i] + j.second + weights[j.first] < minValue ){
					minValue = distL[i] + j.second + weights[j.first];
					u = i;
					v = j.first;
					w = j.second;
				}
			}
		}

		distL[v] = distL[u] + weights[v] + w;
		forest.addEdge(u, v, w);
		distL[u] += w;
		informed.push_back(v);
		uninformed[v] = false; 
	}

	return forest;
}

void WeightedGraph::sortEdges(){
	sort(this->edges.begin(), this->edges.end());
}
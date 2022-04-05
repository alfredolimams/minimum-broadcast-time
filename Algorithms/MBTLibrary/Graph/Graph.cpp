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

// Graph::Graph(int qtNodes) { 
// 	this->adjList.resize(qtNodes);
// 	this->degree.resize(qtNodes);
// 	this->dist.resize(qtNodes);
// }

Graph::~Graph() {

}

vector<int> const Graph::getSources() const {
	return this->sources;
}

bool Graph::addSource(int u){
	return false;
}

int Graph::numberVertices(){
	return 0;
}

int Graph::numberEdges(){
	return 0;
}

vector<int> const & Graph::getDist() const{
	return this->dist;
}

vector<int> const & Graph::getDegree() const{
	return this->degree;
}

double Graph::getDensity(){
	return 0;
}
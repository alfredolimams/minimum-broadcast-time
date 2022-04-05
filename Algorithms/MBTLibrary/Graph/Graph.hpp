/*
 * Graph.hpp
 *
 *  Created on: Jan 27, 2020
 *      Author: Alfredo Lima
 */

#ifndef GRAPH_H
#define GRAPH_H

using namespace std;
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

class Graph {
public:
	Graph();
	~Graph();
	bool addSource(int u);

	vector<int> const getSources() const;
	vector<int> const & getDist() const;
	vector<int> const & getDegree() const;	
	int numberVertices();
	int numberEdges();
	double getDensity();

	virtual void sortEdges() = 0;
private:
	vector<int> sources;
	vector<int> dist;
	vector<int> degree;
};

#endif
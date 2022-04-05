/*
 * SampleDecoder.h
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

class Graph {
public:
	Graph();
	Graph(int qtNodes);
	~Graph();
	bool addEdge(int u, int v);
	bool addGateway(int u);

	set<int> const getNeighbors(int v) const;
	set<int> const getGateways() const;
	int LBB_BFS();
	int numberVertices();
private:
	set<int> gateways;
	vector<set<int>> fathers;
	vector<set<int>> adjList;
};

#endif
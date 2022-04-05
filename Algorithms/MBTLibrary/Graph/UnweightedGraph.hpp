/*
 * UnweightedGraph.hpp
 *
 *  Created on: Dec 20th, 2020
 *      Author: Alfredo Lima
 */

#ifndef UNWEIGHTEDGRAPH_H
#define UNWEIGHTEDGRAPH_H

using namespace std;
#include <vector>
#include <set>
#include <unordered_map>

#include "Graph.hpp"

class UnweightedGraph : public Graph {
public:
	UnweightedGraph();
	UnweightedGraph(int qtNodes);
	~UnweightedGraph();
	bool addEdge(int u, int v);
	bool addSource(int u);

	vector<int> const getNeighbors(int v) const;
	vector<pair<int,int>> const & getEdges() const;
	vector<int> const getSources() const;
	vector<int> const & getDist() const;
	vector<int> const & getDegree() const;	
	int LBB_BFS() const;
	int numberVertices();
	int numberEdges();
	double getDensity();

	void sortEdges();
private:
	vector<int> sources;
	vector<int> dist;
	vector<int> degree;
	vector<vector<int>> adjList;
	vector<pair<int,int>> edges;
};

#endif
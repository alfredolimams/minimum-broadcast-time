/*
 * WeightedGraph.hpp
 *
 *  Created on: Dec 19th, 2020
 *      Author: Alfredo Lima
 */

#ifndef WEIGHTEDGRAPH_H
#define WEIGHTEDGRAPH_H

using namespace std;
#include <vector>
#include <set>
#include <unordered_map>
#include "Graph.hpp"
#include "WeightedForest.hpp"

class WeightedGraph : public Graph {
public:
	WeightedGraph();
	WeightedGraph(int qtNodes, vector<int> & weights);
	~WeightedGraph();
	bool addEdge(int u, int v, int w);
	bool addSource(int u);

	vector<pair<int,int>> const getNeighbors(int v) const;
	vector<tuple<int,int,int>> const & getEdges() const;

	vector<int> const getSources() const;
	vector<int> const & getDist() const;
	vector<int> const & getDegree() const;	
	vector<int> const & getWeights() const;	
	int LBB() const;
	int numberVertices();
	int numberEdges();
	double getDensity();

	WeightedForest HSCHAGreedy();

	void sortEdges();

private:
	vector<int> sources;
	mutable vector<int> dist;
	vector<int> degree;
	vector<int> weights;

	vector<vector<pair<int,int>>> adjList;
	vector<tuple<int,int,int>> edges;
};

#endif
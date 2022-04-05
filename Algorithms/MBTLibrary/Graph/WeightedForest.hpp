/*
 * WeightedForest.hpp
 *
 *  Created on: Dec 20th, 2020
 *      Author: Alfredo Lima
 */

#ifndef WeightedForest_H
#define WeightedForest_H

using namespace std;
#include <vector>
#include <fstream>      // std::ofstream

class WeightedForest {
public:
	WeightedForest();
	WeightedForest(int qtNodes, vector<int> & weights);
	~WeightedForest();
	
	bool addEdge(int u, int v, int w);
	bool addRoot(int u);
	vector<pair<int,int>> const getNeighbors(int v) const;
	vector<tuple<int,int,int>> const getEdges() const;
	vector<vector<int>> const getOrderingTransmissions() const;
	vector<int> const getRoots() const;
	vector<int> const getWeights() const;
	unsigned int const MBT() const;
	int const numberVertices() const;
	int const numberEdges() const;
	int const numberRoots() const;

private:
	mutable unsigned int mbt;
	mutable vector<bool> vstd;
	mutable vector<vector<int>> ordering;
	mutable vector<unsigned int> mbts;

	vector<int> weights;
	vector<int> roots;
	vector<vector<pair<int,int>>> adj_list;
	vector<tuple<int,int,int>> edges;

	unsigned int const scheduleBroadcast(int v) const;
};

#endif
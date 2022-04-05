/*
 * Forest.hpp
 *
 *  Created on: Aug 14, 2020
 *      Author: Alfredo Lima
 */

#ifndef Forest_H
#define Forest_H

using namespace std;
#include <vector>
#include <fstream>      // std::ofstream

class Forest {
public:
	Forest();
	Forest(int qtNodes);
	~Forest();
	
	bool addEdge(int u, int v);
	bool addRoot(int u);
	vector<int> const getNeighbors(int v) const;
	vector<pair<int,int>> const getEdges() const;
	vector<vector<int>> const getOrderingTransmissions() const;
	vector<int> const getRoots() const;
	int const MBT() const;
	int numberVertices();

private:
	mutable int mbt;
	mutable vector<bool> vstd;
	vector<int> roots;
	vector<vector<int>> adj_list;
	vector<pair<int,int>> edges;
	mutable vector<vector<int>> ordering;

	int const scheduleBroadcast(int v) const;
};

#endif
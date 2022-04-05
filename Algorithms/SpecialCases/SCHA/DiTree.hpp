/*
 * DiTree.hpp
 *
 *  Created on: Aug 14, 2020
 *      Author: Alfredo Lima
 */

#ifndef DITREE_H
#define DITREE_H

using namespace std;
#include <vector>
#include <set>
#include <unordered_map>

class DiTree {
public:
	DiTree();
	DiTree(int qtNodes);
	~DiTree();
	bool addEdge(int u, int v);
	bool setRoot(int u);

	set<int> const getNeighbors(int v) const;
	int MBT();
	int numberVertices();
private:
	int scheduleBroadcast(int v);

	int root;
	vector<bool> vstd;
	vector<set<int>> adjList;
};

#endif
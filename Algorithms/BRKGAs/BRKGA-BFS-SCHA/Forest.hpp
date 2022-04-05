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
#include <set>
#include <unordered_map>

class Forest {
public:
	Forest();
	Forest(int qtNodes);
	~Forest();
	bool addEdge(int u, int v);
	bool addRoot(int u);

	set<int> const getNeighbors(int v) const;
	int MBT();
	int numberVertices();
private:
	int scheduleBroadcast(int v);
	vector<bool> vstd;
	set<int> roots;
	vector<set<int>> adjList;
};

#endif
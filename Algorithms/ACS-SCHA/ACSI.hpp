#ifndef ACSI_H_
#define ACSI_H_

#include <bits/stdc++.h>

#include "../MBTLibrary/Graph/UnweightedGraph.hpp"
#include "../MBTLibrary/Graph/Forest.hpp"
#include "../MBTLibrary/MBT/BroadcastSchedule.hpp"
#include "../MBTLibrary/Timer/BossaTimer.hpp"

using namespace std;

class ACSI {
public:
	ACSI(UnweightedGraph * graph, BossaTimer * timer);
	~ACSI();
	BroadcastSchedule MBT(int T, int numberAnts, double tal, double alpha, double beta, double rho, double mutation);
private:

	bool stop();
	void setBestAnswer(int value);
	vector<pair<int,int>> possibleBroadcasts(set<int> & sources);
	BroadcastSchedule buildTreeByAnt(int type, int hasMutation);
	double computeEdgeValue(int type, int V, int i, int j, int p,
							vector<unordered_set<int>> &nm, 
							vector<unordered_set<int>> &nwm, 
							vector<unordered_set<int>> &nwms );

	UnweightedGraph *graph;
	BossaTimer *timer;
	int lb;
	int bestAnswer;
	double timeToBest;
	unordered_map<int, unordered_map<int,double>> pheromone;
};

#endif

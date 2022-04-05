#ifndef BROADCASTSCHEDULE_H_
#define BROADCASTSCHEDULE_H_

#include <bits/stdc++.h>
#include "../Graph/WeightedForest.hpp"

using namespace std;

class BroadcastSchedule {
public:
	BroadcastSchedule();
	BroadcastSchedule(const vector<int> & sources, const vector<vector<int>> & ordering, int mbt);
	BroadcastSchedule(const WeightedForest & forestBroadcast);
	
	~BroadcastSchedule() { schedule.clear(); }

	int const getMBT();
	void printSchedule();
	WeightedForest const getForest() const;
	map<int,vector<pair<int,int>>> const getSchedule() const;

	friend ostream& operator<<(ostream& os, const BroadcastSchedule& bs){
		map<int,vector<pair<int,int>>> schedule = bs.getSchedule();
		for ( auto kv : schedule ) {
			os << "Time " << kv.first << ": ";
			for (unsigned j = 0; j < kv.second.size() ; ++j) {
				if (j) os << ", ";
				os << kv.second[j].first+1 << "-" << kv.second[j].second+1;
			}
			os << endl;
		}
		return os;
	}

private:
	WeightedForest solution;
	map<int,vector<pair<int,int>>> schedule;
	int mbt;
};

#endif
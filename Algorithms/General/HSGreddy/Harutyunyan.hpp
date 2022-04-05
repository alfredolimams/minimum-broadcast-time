#ifndef HSCHA_H_
#define HSCHA_H_

#include "../../MBTLibrary/Graph/WeightedGraph.hpp"
#include "../../MBTLibrary/Graph/WeightedForest.hpp"
#include "../../MBTLibrary/MBT/BroadcastSchedule.hpp"

class HSCHAGreedy {
public:
	HSCHAGreedy(WeightedGraph * graph);
	~HSCHAGreedy();
	WeightedForest mbt();
private:
	WeightedGraph * graph;
};

#endif /* HARUTYUNYAN_H_ */
#ifndef HARUTYUNYAN_H_
#define HARUTYUNYAN_H_

#include "../../MBTLibrary/Graph/WeightedGraph.hpp"
#include "../../MBTLibrary/MBT/BroadcastSchedule.hpp"

class HarutyunyanGreedy {
public:
	HarutyunyanGreedy(WeightedGraph * graph);
	~HarutyunyanGreedy();
	int mbt();
private:
	WeightedGraph * graph;
};

#endif /* HARUTYUNYAN_H_ */
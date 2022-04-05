#ifndef GRASP_H_
#define GRASP_H_

#include "../../MBTLibrary/Graph/WeightedGraph.hpp"
#include "../../MBTLibrary/Graph/WeightedForest.hpp"
#include "../../MBTLibrary/MBT/BroadcastSchedule.hpp"
#include "../../MBTLibrary/Timer/BossaTimer.hpp"

#include "MTRand.hpp"


class GRASP {
public:
	GRASP(WeightedGraph * graph, MTRand * rand, BossaTimer * timer);
	~GRASP();
	WeightedForest mbt(double alpha);
	WeightedForest compute(unsigned T, double timeout, double alpha);
private:
	bool stop(unsigned v, unsigned t, unsigned T);
	WeightedGraph * graph;
	MTRand * rand;
	BossaTimer * timer;
	unsigned lb;
	double ttt;
};

#endif /* HARUTYUNYAN_H_ */
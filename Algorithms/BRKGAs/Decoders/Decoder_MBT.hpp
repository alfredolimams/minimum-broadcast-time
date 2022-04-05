/*
 * Decoder_MBT.h
 *
 * Any decoder must have the format below, i.e., implement the method decode(std::vector< double >&)
 * returning a double corresponding to the fitness of that vector. If parallel decoding is to be
 * used in the BRKGA framework, then the decode() method _must_ be thread-safe; the best way to
 * guarantee this is by adding 'const' to the end of decode() so that the property will be checked
 * at compile time.
 *
 * The chromosome inside the BRKGA framework can be changed if desired. To do so, just use the
 * first signature of decode() which allows for modification. Please use double values in the
 * interval [0,1) when updating, thus obeying the BRKGA guidelines.
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#ifndef DECODER_MBT_H
#define DECODER_MBT_H

#include <bits/stdc++.h>

// Graph Libraries
#include "../../MBTLibrary/Graph/UnweightedGraph.hpp"
#include "../../MBTLibrary/Graph/WeightedGraph.hpp"
#include "../../MBTLibrary/Graph/Forest.hpp"

// Timer Libraries
#include "../../MBTLibrary/Timer/BossaTimer.hpp"

// MBT Libraries
#include "../../MBTLibrary/MBT/BroadcastSchedule.hpp"

using namespace std;

class Decoder_MBT {
public:
	Decoder_MBT();
	virtual ~Decoder_MBT();

	bool init(UnweightedGraph * graph, BossaTimer * timer, int lb);
	virtual double decode(const vector< double >& chromosome) const = 0;
	virtual BroadcastSchedule const broadcast_schedule(const vector< double >& chromosome) const = 0;
	virtual vector<double> const broadcast_to_cromossome(BroadcastSchedule const schedule) const = 0;
	
	void setBestAnswer(int value) const;
	bool stop() const;
	int getBestAnswer();
	double getTimeAnswer();

	double getPe();
	double getPm();
	double getRhoe();

protected:
	mutable int bestAnswer;
	mutable double timeAnswer;

	int lb;
	UnweightedGraph * graph;
	BossaTimer * timer;
	//
	double pe;
	double pm;
	double rhoe;
};

#endif

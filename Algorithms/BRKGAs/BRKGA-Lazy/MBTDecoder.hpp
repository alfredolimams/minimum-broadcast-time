/*
 * MBTDecoder.h
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

#ifndef MBTDECODER_H
#define MBTDECODER_H

#include <list>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>

#include "Graph.hpp"
#include "ArgPack.hpp"
#include "BossaTimer.hpp"

using namespace std;

class MBTDecoder {
public:
	MBTDecoder();
	~MBTDecoder();

	bool init(Graph * graph, BossaTimer * timer);
	double decode(const vector< double >& chromosome) const;
	bool stop() const;
	int getBestAnswer();
	void setBestAnswer(int value) const;

private:
	int lb;
	Graph * graph;
	BossaTimer * timer;
};

#endif

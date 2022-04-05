/*
 * Decoder_FRFS.h
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

#ifndef Decoder_FRFS0_H
#define Decoder_FRFS0_H

#include "Decoder_MBT.hpp"

using namespace std;

class Decoder_FRFS0 : public Decoder_MBT {
public:
	Decoder_FRFS0(string name);
	Decoder_FRFS0(double pe, double pm, double rhoe);
	~Decoder_FRFS0();
	vector<double> const broadcast_to_cromossome(BroadcastSchedule const schedule) const;
	double decode(const vector< double >& chromosome) const;
	BroadcastSchedule const broadcast_schedule(const std::vector< double >& chromosome) const;
};

#endif

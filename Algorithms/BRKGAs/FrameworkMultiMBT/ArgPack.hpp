#ifndef ARGPACK_H_
#define ARGPACK_H_

#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

class ArgPack {
public:
	unsigned decoders;
	string inputFile;
	vector<string> outputFiles;
	vector<string> typeDecoders;

	unsigned population; // Size of population
	unsigned K; // Number of independent populations
	vector<vector<double>> configs;
	
	unsigned threads; // Number of threads for parallel decoding
	double t; // Time limit

	unsigned rngSeed; // Seed random number generator
	unsigned exchangeBest; // Exchange best individuals
	unsigned exchangeTop; // Exchange top 2 best
	unsigned generations; // Run for 1000 generations

	bool printOutput;
	bool writeFile;
	bool verbose;

	int result; 

	//------------
	// singleton functions
	//------------

	static const ArgPack &getInstance() { assert(def_ap_); return *def_ap_; }

	void print() const;

	bool Ok() const;


	ArgPack(int argc, char * const argv []);

	~ArgPack() { assert(def_ap_); def_ap_ = 0; }

private:

	//------------
	// singleton instance
	//------------
	
	static ArgPack *def_ap_;

	string value;
	bool ok;
};



#endif /* ARGPACK_H_ */
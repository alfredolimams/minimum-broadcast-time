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
	string inputFile;
	string outputFile;

	string typeDecoder;
	// unsigned population; // Size of population
	double populationElite; // Fraction of population to be the elite-set
	double populationMutants; // Fraction of population to be replaced by mutants
	double rhoe; // Probability offspring inherits elite parent allele
	double alpha;
	double beta;
	unsigned K; // Number of independent populations
	unsigned threads; // Number of threads for parallel decoding
	double t; // Time limit

	unsigned rngSeed; // Seed random number generator
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
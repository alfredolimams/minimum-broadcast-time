#ifndef ARGPACK_H_
#define ARGPACK_H_

#include <string>
#include <assert.h>

using namespace std;

class ArgPack {
public:
	string inputFile;
	string outputFile;

	unsigned rngSeed; // Seed random number generator
	unsigned generations; // Run for 1000 generations
	// unsigned population; // Size of ants
	double mutants; // Fraction of population to be replaced by mutants
	
	double tal; // Value of tal
	double alpha; // Value of alpha
	double beta; // Value of beta
	double rho; // Value of rho

	double t; // Time limit

	bool printOutput;
	bool writeFile;

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
	string programName;
	bool ok;
};



#endif /* ARGPACK_H_ */
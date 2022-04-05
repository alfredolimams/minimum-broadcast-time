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

	bool printOutput;
	bool writeFile;
	bool verbose;

	double t; // Time limit
	unsigned rngSeed; // Seed random number generator
	unsigned generations; // Run for 1000 generations
	double alpha;

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
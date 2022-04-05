#include "ArgPack.hpp"

#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>
// #include "../MBTLibrary/BossaTimer.hpp"

using namespace std;

ArgPack *ArgPack::def_ap_ = 0;

static void show_usage(string name)
{
    cerr << "Usage: " << name << " <option(s)> SOURCES";
}

ArgPack::ArgPack(int argc, char * const argv []) :
	inputFile(""),
	outputFile(""),
	
	rngSeed(0),
	generations(1000),
	// population(250),
	mutants(0.05),

	tal(1),
	alpha(20),
	beta(20),
	rho(0.1),

	t(0.0),

	printOutput(false),
	writeFile(false){

	assert(!def_ap_);
	def_ap_ = this;
	programName = argv[0];
	ok = false;
	bool error = false;

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];
		if( arg == "-h" or arg == "--help" ){
			show_usage(programName);
		} else if(arg == "-i" or arg == "--input") {
			try {
				inputFile = argv[++i];
			} 
			catch (...)	{
				cerr << "[INPUT] Unknown exception caught!" << endl;
				error = true;
			}
		} else if (arg == "-o" or arg == "--output"){
			try {
				outputFile = argv[++i];
			} 
			catch (...)	{
				cerr << "[OUTPUT] Unknown exception caught!" << endl;
				error = true;
			}
		} else if(arg == "-s" or arg == "--seed"){
			try {
				value = argv[++i];
				rngSeed = strtoul(value.c_str(),NULL,10);
			} 
			catch (...)	{
				cerr << "[SEED] Unknown exception caught!" << endl;
				error = true;
			}
		} else if(arg == "-m" or arg == "--mutants"){
			try {
				value = argv[++i];
				mutants = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[POPULATION MUTANTS] Unknown exception caught!" << endl;
			}
		} else if(arg == "-r" or arg == "--rho"){
			try {
				value = argv[++i];
				rho = stof(value);
			} 
			catch (...)	{
				cerr << "[RHO] Unknown exception caught!" << endl;
				error = true;
			}
		} else if(arg == "-t" or arg == "--time"){
			try {
				value = argv[++i];
				t = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[TIME] Unknown exception caught!" << endl;
			}
		} else if(arg == "-T" or arg == "--tal"){
			try {
				value = argv[++i];
				tal = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[TIME] Unknown exception caught!" << endl;
			}
		} else if(arg == "-a" or arg == "--alpha"){
			try {
				value = argv[++i];
				alpha = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[TIME] Unknown exception caught!" << endl;
			}
		} else if(arg == "-b" or arg == "--beta"){
			try {
				value = argv[++i];
				beta = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[TIME] Unknown exception caught!" << endl;
			}
		} else if(arg == "-g" or arg == "--generations"){
			try {
				value = argv[++i];
				generations = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[GENERATIONS] Unknown exception caught!" << endl;
			}
		} else if(arg == "-S" or arg == "--show"){
			try {
				printOutput = true;
			} 
			catch (...)	{
				error = true;
				cerr << "[SHOW] Unknown exception caught!" << endl;
			}
		} else if(arg == "-w" or arg == "--writeFile"){
			try {
				writeFile = true;
			} 
			catch (...)	{
				error = true;
				cerr << "[WRITE FILE] Unknown exception caught!" << endl;
			}
		} else {
			error = true;
			cerr << "Unexpected parameter!" << endl;
		}
	}

	if( !error ){
		if( inputFile == "" ){
			error = true;
			cerr << "Missing parameters!" << endl;
		}
		if( !error ){
			if( outputFile == "" ){
				int pos = inputFile.find_last_of('.');
				if( pos < 0 ){
					outputFile = inputFile + "_H.out";
				} else {
					outputFile = inputFile.substr(0,pos) + "_H.out";
				}
			}
			ok = true;
		}
	}
}

bool ArgPack::Ok() const {
	return ok;
}

void ArgPack::print() const{
}

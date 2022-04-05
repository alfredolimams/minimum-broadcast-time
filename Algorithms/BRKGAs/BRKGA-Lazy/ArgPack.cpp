#include "ArgPack.hpp"

#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "BossaTimer.hpp"

using namespace std;

ArgPack *ArgPack::def_ap_ = 0;

static void show_usage(string name)
{
    cerr << "Usage: " << name << " <option(s)> SOURCES";
}

ArgPack::ArgPack(int argc, char * const argv []) :
	inputFile(""),
	outputFile(""),
	
	population(250),
	populationElite(0.22),
	populationMutants(0.26),
	rhoe(0.66),
	K(9),
	threads(4),
	t(0.0),

	rngSeed(0),
	exchangeBest(100),
	exchangeTop(20),
	generations(1000),
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
		} else if(arg == "-p" or arg == "--population"){
			try {
				value = argv[++i];
				population = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[POPULATION] Unknown exception caught!" << endl;
			}
		} else if(arg == "-e" or arg == "--populationElite"){
			try {
				value = argv[++i];
				populationElite = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[POPULATION ELITE] Unknown exception caught!" << endl;
			}
		} else if(arg == "-m" or arg == "--populationMutants"){
			try {
				value = argv[++i];
				populationMutants = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[POPULATION MUTANTS] Unknown exception caught!" << endl;
			}
		} else if(arg == "-r" or arg == "--rhoe"){
			try {
				value = argv[++i];
				rhoe = stof(value);
			} 
			catch (...)	{
				cerr << "[RHOE] Unknown exception caught!" << endl;
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
		} else if(arg == "-t" or arg == "--time"){
			try {
				value = argv[++i];
				t = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[TIME] Unknown exception caught!" << endl;
			}
		} else if(arg == "-K" or arg == "--K"){
			try {
				value = argv[++i];
				K = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[K] Unknown exception caught!" << endl;
			}
		} else if(arg == "-T" or arg == "--threads"){
			try {
				value = argv[++i];
				threads = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[THREAD] Unknown exception caught!" << endl;
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
		} else if(arg == "-b" or arg == "--exchangeBest"){
			try {
				value = argv[++i];
				exchangeBest = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[EXCHANGE BEST] Unknown exception caught!" << endl;
			}
		} else if(arg == "-x" or arg == "--exchangeTop"){
			try {
				value = argv[++i];
				exchangeTop = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[EXCHANGE TOP] Unknown exception caught!" << endl;
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

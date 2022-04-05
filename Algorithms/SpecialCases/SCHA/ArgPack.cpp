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
					outputFile = inputFile + "_Slater.out";
				} else {
					outputFile = inputFile.substr(0,pos) + "_Slater.out";
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

#include "ArgPack.hpp"

using namespace std;

ArgPack *ArgPack::def_ap_ = 0;

ArgPack::ArgPack(int argc, char * const argv []) :
	// Files
	inputFile(""),
	outputFile(""),

	printOutput(false),
	writeFile(false),
	verbose(false)
	{
	assert(!def_ap_);
	def_ap_ = this;
	// programName = argv[0];
	ok = false;
	bool error = false;

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];
		if( arg == "-h" or arg == "--help" ){
			// show_usage(programName);
		} else if(arg == "-i" or arg == "--input") {
			try {
				inputFile = argv[++i];
			} 
			catch (...)	{
				cerr << "[INPUT] Unknown exception caught!" << endl;
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
		} else if(arg == "-V" or arg == "--verbose"){
			try {
				verbose = true;
			} 
			catch (...)	{
				error = true;
				cerr << "[VERBOSE] Unknown exception caught!" << endl;
			}
		} else if(arg == "-w" or arg == "--writeFile"){
			try {
				writeFile = true;
			} 
			catch (...)	{
				error = true;
				cerr << "[WRITE FILE] Unknown exception caught!" << endl;
			}
		} else if(arg == "-o" or arg == "--output"){
			try {
				outputFile = argv[++i];
			} 
			catch (...)	{
				error = true;
				cerr << "[OUTPUT] Unknown exception caught!" << endl;
			}
		} else {
			error = true;
			cerr << "Unexpected parameter! [" << argv[i] << "]" << endl;
		}
	}

	if( !error ){
		if( inputFile == "" ){
			error = true;
			cerr << "Missing parameters!" << endl;
		}
		ok = !error;
	}
}

bool ArgPack::Ok() const {
	return ok;
}

void ArgPack::print() const{
}
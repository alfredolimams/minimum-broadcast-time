#include "ArgPack.hpp"

using namespace std;

ArgPack *ArgPack::def_ap_ = 0;

ArgPack::ArgPack(int argc, char * const argv []) :
	// Files
	inputFile(""),
	outputFile(""),
	typeDecoder("FRFSI0"),
	// population(250),
	populationElite(0.16),
	populationMutants(0.10),
	rhoe(0.74),
	alpha(0.54),
	beta(0.44),
	K(1),
	threads(1),
	t(0.0),

	rngSeed(0),
	generations(1000),
	printOutput(false),
	writeFile(false),
	verbose(false),
	result(0)
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
		} else if(arg == "-R" or arg == "--result"){
			try {
				value = argv[++i];
				result = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[RESULT] Unknown exception caught!" << endl;
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
		} else if(arg == "-d" or arg == "--decoder"){
			try {
				typeDecoder = argv[++i];
			} 
			catch (...)	{
				error = true;
				cerr << "[DECODERS] Unknown exception caught!" << endl;
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
		} else if(arg == "-a" or arg == "--alpha"){
			try {
				value = argv[++i];
				alpha = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[ALPHA] Unknown exception caught!" << endl;
			}
		} else if(arg == "-b" or arg == "--beta"){
			try {
				value = argv[++i];
				beta = stof(value);
			} 
			catch (...)	{
				error = true;
				cerr << "[ALPHA] Unknown exception caught!" << endl;
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
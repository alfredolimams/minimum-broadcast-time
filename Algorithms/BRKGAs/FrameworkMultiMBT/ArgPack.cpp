#include "ArgPack.hpp"

using namespace std;

ArgPack *ArgPack::def_ap_ = 0;

ArgPack::ArgPack(int argc, char * const argv []) :
	// Files
	decoders(1),
	inputFile(""),
	outputFiles(vector<string>()),
	typeDecoders(vector<string>()),
	// setupDecoders(vector<string>()),
	population(250),
	K(1),
	configs(vector<vector<double>>()),
	threads(1),
	t(0.0),

	rngSeed(0),
	exchangeBest(100),
	exchangeTop(20),
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
		} else if(arg == "-p" or arg == "--population"){
			try {
				value = argv[++i];
				population = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[POPULATION] Unknown exception caught!" << endl;
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
		} else if(arg == "-d" or arg == "--decoders"){
			try {
				value = argv[++i];
				decoders = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[DECODERS] Unknown exception caught!" << endl;
			}
		} else if(arg == "-c" or arg == "--config"){
			try {
				for (unsigned d = 0; d < decoders; ++d) {
					typeDecoders.push_back(argv[++i]);
					vector<double> config;
					// Elite
					// Mutants
					// Rhoe
					for (int j = 0; j < 3; ++j) {
						value = argv[++i];
						config.push_back(stof(value));		
					}
					// Alpha
					if ( typeDecoders[d].find("1") != string::npos or typeDecoders[d].find("3") != string::npos ){
						value = argv[++i];
						config.push_back(stof(value));
					}
					// Beta
					if ( typeDecoders[d].find("2") != string::npos or typeDecoders[d].find("3") != string::npos ){
						value = argv[++i];
						config.push_back(stof(value));
					}
					configs.push_back(config);
				}
			} 
			catch (...)	{
				error = true;
				cerr << "[CONFIG] Unknown exception caught!" << endl;
			}
		} else if(arg == "-S" or arg == "--show"){
			try {
				printOutput = true;
			} 
			catch (...)	{
				error = true;
				cerr << "[SHOW] Unknown exception caught!" << endl;
			}
		}  else if(arg == "-V" or arg == "--verbose"){
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
		} else if(arg == "-R" or arg == "--result"){
			try {
				value = argv[++i];
				result = stoi(value, nullptr, 10);
			} 
			catch (...)	{
				error = true;
				cerr << "[RESULT] Unknown exception caught!" << endl;
			}
		} else if(arg == "-o" or arg == "--output"){
			try {
				for (unsigned d = 0; d < decoders; ++d) {
					outputFiles.push_back(argv[++i]);
				}
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
		if( !error ){
			if( writeFile ){
				// error = outputFiles.size() != setupDecoders.size();
				// error |= outputFiles.size() != typeDecoders.size();
				// error |= setupDecoders.size() != decoders;
			}
			ok = !error;
		}
	}
}

bool ArgPack::Ok() const {
	return ok;
}

void ArgPack::print() const{
}